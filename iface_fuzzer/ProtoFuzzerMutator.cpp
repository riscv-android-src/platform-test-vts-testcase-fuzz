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

#include "ProtoFuzzerMutator.h"
#include "type_mutators/ProtoFuzzerEnumMutator.h"
#include "type_mutators/ProtoFuzzerScalarMutator.h"
#include "type_mutators/ProtoFuzzerStructMutator.h"

#include <iostream>

using std::endl;
using std::cerr;
using std::cout;
using std::make_unique;

namespace android {
namespace vts {

ProtoFuzzerMutator::ProtoFuzzerMutator(
    Random &rand,
    unordered_map<string, VariableSpecificationMessage> predefined_types)
    : rand_(rand), predefined_types_(predefined_types) {
  type_mutators_[TYPE_ENUM] =
      make_unique<ProtoFuzzerEnumMutator>(rand, this, predefined_types_);
  type_mutators_[TYPE_SCALAR] =
      make_unique<ProtoFuzzerScalarMutator>(rand, this, predefined_types_);
  type_mutators_[TYPE_STRUCT] =
      make_unique<ProtoFuzzerStructMutator>(rand, this, predefined_types_);
}

ExecutionSpecificationMessage ProtoFuzzerMutator::RandomGen(
    const InterfaceSpecificationMessage &iface_spec, size_t num_calls) {
  ExecutionSpecificationMessage result{};

  for (size_t i = 0; i < num_calls; ++i) {
    size_t num_apis = iface_spec.api_size();
    size_t rand_api_idx = rand_(num_apis);
    FunctionSpecificationMessage rand_api =
        RandomGen(iface_spec.api(rand_api_idx));
    result.add_api()->Swap(&rand_api);
  }

  return result;
}

void ProtoFuzzerMutator::Mutate(const InterfaceSpecificationMessage &iface_spec,
                                ExecutionSpecificationMessage *exec_spec) {
  bool coin_flip = rand_(2);

  if (coin_flip) {
    size_t idx = rand_(exec_spec->api_size());
    const FunctionSpecificationMessage &rand_api = exec_spec->api(idx);
    (*exec_spec->mutable_api(idx)) = Mutate(rand_api);
  } else {
    size_t num_apis = iface_spec.api_size();
    size_t idx = rand_(num_apis);
    (*exec_spec->mutable_api(idx)) = RandomGen(iface_spec.api(idx));
  }
}

FunctionSpecificationMessage ProtoFuzzerMutator::RandomGen(
    const FunctionSpecificationMessage &func_spec) {
  FunctionSpecificationMessage result{func_spec};
  // We'll repopulate arg field.
  result.clear_arg();

  for (const auto &var_spec : func_spec.arg()) {
    VariableSpecificationMessage rand_var_spec = RandomGen(var_spec);
    auto *new_var = result.add_arg();
    new_var->Swap(&rand_var_spec);
  }

  return result;
}

FunctionSpecificationMessage ProtoFuzzerMutator::Mutate(
    const FunctionSpecificationMessage &func_spec) {
  FunctionSpecificationMessage result{func_spec};

  size_t num_args = result.arg_size();
  if (num_args > 0) {
    size_t rand_arg_idx = rand_(num_args);
    VariableSpecificationMessage rand_arg = Mutate(result.arg(rand_arg_idx));
    result.mutable_arg(rand_arg_idx)->Swap(&rand_arg);
  }
  return result;
}

VariableSpecificationMessage ProtoFuzzerMutator::RandomGen(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{
      type_mutators_.at(var_spec.type())->RandomGen(var_spec)};
  return result;
}

VariableSpecificationMessage ProtoFuzzerMutator::Mutate(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{
      type_mutators_.at(var_spec.type())->Mutate(var_spec)};
  return result;
}

}  // namespace vts
}  // namespace android
