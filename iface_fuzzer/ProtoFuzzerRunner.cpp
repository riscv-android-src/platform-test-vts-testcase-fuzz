/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ProtoFuzzerRunner.h"

#include <dlfcn.h>
#include <sstream>

#include "utils/InterfaceSpecUtil.h"
#include "vintf/HalManifest.h"
#include "vintf/VintfObject.h"

using android::vintf::HalManifest;

using std::cout;
using std::cerr;
using std::string;
using std::vector;
using std::unordered_map;

namespace android {
namespace vts {
namespace fuzzer {

static string GetDriverName(const CompSpec &comp_spec) {
  stringstream version;
  version.precision(1);
  version << fixed << comp_spec.component_type_version();
  string driver_name =
      comp_spec.package() + "@" + version.str() + "-vts.driver.so";
  return driver_name;
}

static string GetServiceName(const CompSpec &comp_spec) {
  static const HalManifest *vendor_manifest =
      ::android::vintf::VintfObject::GetDeviceHalManifest();
  string hal_name = comp_spec.package();
  string iface_name = comp_spec.component_name();

  auto instance_names = vendor_manifest->getInstances(hal_name, iface_name);
  if (instance_names.empty()) {
    cerr << "HAL service name not available in VINTF." << endl;
    exit(1);
  }

  // For fuzzing we don't care which instance of the HAL is targeted.
  string service_name = *instance_names.begin();
  cout << "Available HAL instances: " << endl;
  for (const string &instance_name : instance_names) {
    cout << instance_name << endl;
  }
  cout << "Using HAL instance: " << service_name << endl;

  return service_name;
}

static FuzzerBase *InitHalDriver(const CompSpec &comp_spec, bool binder_mode) {
  const char *error;
  string driver_name = GetDriverName(comp_spec);
  void *handle = dlopen(driver_name.c_str(), RTLD_LAZY);
  if (!handle) {
    cerr << __func__ << ": " << dlerror() << endl;
    cerr << __func__ << ": Can't load shared library: " << driver_name << endl;
    exit(1);
  }

  // Clear dlerror().
  dlerror();
  string function_name = GetFunctionNamePrefix(comp_spec);
  using loader_func = FuzzerBase *(*)();
  auto hal_loader = (loader_func)dlsym(handle, function_name.c_str());
  if ((error = dlerror()) != NULL) {
    cerr << __func__ << ": Can't find: " << function_name << endl;
    cerr << error << endl;
    exit(1);
  }

  FuzzerBase *hal = hal_loader();
  string service_name = GetServiceName(comp_spec);
  cerr << "HAL name: " << comp_spec.package() << endl
       << "Interface name: " << comp_spec.component_name() << endl
       << "Service name: " << service_name << endl;

  // For fuzzing, only passthrough mode provides coverage.
  // If binder mode is not requested, attempt to open HAL in passthrough mode.
  // If the attempt fails, fall back to binder mode.
  if (!binder_mode) {
    if (!hal->GetService(true, service_name.c_str())) {
      cerr << __func__ << ": Failed to open HAL in passthrough mode. "
           << "Falling back to binder mode." << endl;
    } else {
      cerr << "HAL opened in passthrough mode." << endl;
      return hal;
    }
  }

  if (!hal->GetService(false, service_name.c_str())) {
    cerr << __func__ << ": Failed to open HAL in binder mode." << endl;
    exit(1);
  } else {
    cerr << "HAL opened in binder mode." << endl;
  }
  return hal;
}

ProtoFuzzerRunner::ProtoFuzzerRunner(const vector<CompSpec> &comp_specs) {
  for (const auto &comp_spec : comp_specs) {
    if (comp_spec.has_interface()) {
      string name = comp_spec.component_name();
      comp_specs_[name] = comp_spec;
    }
  }
}

void ProtoFuzzerRunner::Init(const string &iface_name, bool binder_mode) {
  const CompSpec *comp_spec = FindCompSpec(iface_name);
  std::shared_ptr<FuzzerBase> hal{InitHalDriver(*comp_spec, binder_mode)};

  // Register this interface as opened by the runner.
  opened_ifaces_[iface_name] = {
      .comp_spec_ = comp_spec, .hal_ = hal,
  };
}

void ProtoFuzzerRunner::Execute(const ExecSpec &exec_spec) {
  for (const auto &iface_func_call : exec_spec.function_call()) {
    string iface_name = iface_func_call.hidl_interface_name();
    const FuncSpec &func_spec = iface_func_call.api();

    auto iface_desc = opened_ifaces_.find(iface_name);
    if (iface_desc == opened_ifaces_.end()) {
      cerr << "Interface is not open: " << iface_name << endl;
      exit(1);
    }
    cout << iface_func_call.DebugString() << endl;

    FuncSpec result{};
    iface_desc->second.hal_->CallFunction(func_spec, "", &result);
  }
}

const CompSpec *ProtoFuzzerRunner::FindCompSpec(std::string name) {
  auto comp_spec = comp_specs_.find(name);
  if (comp_spec == comp_specs_.end()) {
    cerr << "VTS spec not found: " << name << endl;
    exit(1);
  }
  return &comp_spec->second;
}

}  // namespace fuzzer
}  // namespace vts
}  // namespace android
