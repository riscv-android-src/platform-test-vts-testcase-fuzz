/*
 * Copyright 2019 The Android Open Source Project
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

#include <vintf/VintfObject.h>

#define STRINGIFY(x) STRINGIFY_INTERNAL(x)
#define STRINGIFY_INTERNAL(x) #x

using android::FQName;
using android::vintf::Version;
using android::vintf::VintfObject;
using std::cerr;
using std::cout;
using std::string;

namespace android {
namespace vts {
namespace fuzzer {

// TODO(b/145220086): fuzzer should attempt to fuzz all interfaces and instances
// it can find.
static FQName FindAnyIfaceFQName(const FQName &package_and_version,
                                 const vector<CompSpec> &comp_specs) {
  for (const auto &spec : comp_specs) {
    auto package = package_and_version.package();
    auto major_version = package_and_version.getPackageMajorVersion();
    auto minor_version = package_and_version.getPackageMinorVersion();

    if (package == spec.package() &&
        major_version == spec.component_type_version_major() &&
        minor_version == spec.component_type_version_minor()) {
      auto iface_name = spec.component_name();
      auto instance_names =
          VintfObject::GetDeviceHalManifest()->getHidlInstances(
              package,
              Version(spec.component_type_version_major(),
                      spec.component_type_version_minor()),
              iface_name);

      if (!instance_names.empty()) {
        auto version =
            std::to_string(major_version) + "." + std::to_string(minor_version);
        return FQName{package, version, iface_name};
      }
    }
  }
  return FQName{};
}

ProtoFuzzerParams ExtractProtoFuzzerStaticParams(int argc, char **argv) {
  ProtoFuzzerParams params = ExtractProtoFuzzerParams(argc, argv);
  FQName package_and_version;

  if (!FQName::parse(STRINGIFY(STATIC_TARGET_FQ_NAME), &package_and_version)) {
    cerr << "STATIC_TARGET_FQ_NAME is malformed" << endl;
    std::abort();
  }

  // Find first interface in the given package that fits the bill.
  params.target_fq_name_ =
      FindAnyIfaceFQName(package_and_version, params.comp_specs_);
  if (!params.target_fq_name_.isFullyQualified()) {
    cerr << "HAL service name not available in VINTF." << endl;
    std::abort();
  }

  // Hard-coded values
  params.exec_size_ = 16;
  return params;
}

}  // namespace fuzzer
}  // namespace vts
}  // namespace android
