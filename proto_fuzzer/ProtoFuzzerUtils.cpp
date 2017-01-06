/*
 * Copyright 2016 The Android Open Source Project
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

#include "ProtoFuzzerUtils.h"

#include <sstream>

#include <getopt.h>

#include "dlfcn.h"
#include "specification_parser/InterfaceSpecificationParser.h"
#include "utils/InterfaceSpecUtil.h"

using std::cout;
using std::cerr;
using std::string;
using std::unordered_map;
using std::vector;

static void usage() {
  fprintf(
      stdout,
      "Usage:\n"
      "\n"
      "./<fuzzer> <vts flags> -- <libfuzzer flags>\n"
      "\n"
      "VTS flags (strictly in form --flag=value):\n"
      "\n"
      " vts_spec_files \tColumn-separated list of paths to vts spec files.\n"
      " vts_exec_size \t\tNumber of function calls per fuzzer execution.\n"
      "\n"
      "libfuzzer flags (strictly in form -flag=value):\n"
      " Use -help=1 to see libfuzzer flags\n"
      "\n"
      );
}

static struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"vts_spec_files", required_argument, 0, 's'},
    {"vts_exec_size", required_argument, 0, 'e'}};

namespace android {
namespace vts {

vector<ComponentSpecificationMessage> ExtractCompSpecs(
    const char *vts_spec_files) {
  vector<ComponentSpecificationMessage> comp_specs;
  string input(vts_spec_files);
  std::istringstream iss(input);

  string vts_spec_file;
  while (std::getline(iss, vts_spec_file, ':')) {
    ComponentSpecificationMessage comp_spec;
    InterfaceSpecificationParser::parse(vts_spec_file.c_str(), &comp_spec);
    comp_specs.emplace_back(std::move(comp_spec));
  }

  return comp_specs;
}

ProtoFuzzerParams ExtractProtoFuzzerParams(int argc, char **argv) {
  ProtoFuzzerParams params;
  int opt = 0;
  int index = 0;
  while ((opt = getopt_long_only(argc, argv, "", long_options, &index)) != -1) {
    switch (opt) {
      case 'h':
        usage();
        exit(0);
      case 's':
        params.comp_specs_ = ExtractCompSpecs(optarg);
        break;
      case 'e':
        params.exec_size_ = atoi(optarg);
        break;
      default:
        // Ignore. This option will be handled by libfuzzer.
        break;
    }
  }
  return params;
}

// TODO(trong): this should be done using FuzzerWrapper.
FuzzerBase *InitHalDriver(const ComponentSpecificationMessage &comp_spec) {
  stringstream version;
  version.precision(1);
  version << fixed << comp_spec.component_type_version();
  string driver_name =
      comp_spec.package() + ".vts.driver@" + version.str() + ".so";

  void *handle = dlopen(driver_name.c_str(), RTLD_LAZY);

  typedef FuzzerBase *(*loader_func)();
  auto hal_loader =
      (loader_func)dlsym(handle, GetFunctionNamePrefix(comp_spec).c_str());

  // Infer HAL service name from its package name.
  string prefix = "android.hardware.";
  string service_name = comp_spec.package().substr(prefix.size());

  FuzzerBase *hal = hal_loader();
  // For fuzzing, we always use passthrough mode.
  hal->GetService(true, service_name.c_str());

  return hal;
}

unordered_map<string, VariableSpecificationMessage> ExtractPredefinedTypes(
    const vector<ComponentSpecificationMessage> &specs) {
  unordered_map<string, VariableSpecificationMessage> predefined_types;
  for (const auto &comp_spec : specs) {
    for (const auto &var_spec : comp_spec.attribute()) {
      predefined_types[var_spec.name()] = var_spec;
    }
  }
  return predefined_types;
}

void Execute(FuzzerBase *hal, const ExecutionSpecificationMessage &exec_spec) {
  FunctionSpecificationMessage result{};
  for (const auto &func_spec : exec_spec.api()) {
    hal->CallFunction(func_spec, "", &result);
  }
}

}  // namespace vts
}  // namespace android
