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

#include "type_mutators/ProtoFuzzerUnionMutator.h"

namespace android {
namespace vts {

VariableSpecificationMessage ProtoFuzzerUnionMutator::RandomGen(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{};
  result.set_name(var_spec.predefined_type());
  result.set_type(TYPE_UNION);

  const VariableSpecificationMessage &blueprint =
      FindPredefinedType(result.name());

  // TODO(trong): once vts driver supports TYPE_UNION, there should be only 1
  // union_value.
  for (const auto &union_value : blueprint.union_value()) {
    *result.add_union_value() = mutator_->RandomGen(union_value);
  }

  /*
  // Generate only 1 randomly chosen union_value field.
  size_t size = var_spec.union_value_size();
  size_t idx = rand_(size);
  *result.add_union_value() =
      mutator_->RandomGen(blueprint.union_value(idx));
  */
  return result;
}

VariableSpecificationMessage ProtoFuzzerUnionMutator::Mutate(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{var_spec};
  // Assume contains exactly 1 union_value field.
  *result.mutable_union_value(0) = mutator_->Mutate(var_spec.union_value(0));
  return result;
}

}  // namespace vts
}  // namespace android
