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

using std::cout;
using std::cerr;
using std::string;
using std::unordered_map;
using std::vector;

namespace android {
namespace vts {

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
