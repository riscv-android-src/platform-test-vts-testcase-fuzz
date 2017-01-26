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

#include "type_mutators/ProtoFuzzerEnumMutator.h"

using std::string;

namespace android {
namespace vts {

VariableSpecificationMessage ProtoFuzzerEnumMutator::RandomGen(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result;
  string name = (var_spec.has_predefined_type()) ? var_spec.predefined_type()
                                                 : var_spec.name();
  result.set_name(name);
  result.set_type(TYPE_ENUM);

  const EnumDataValueMessage &blueprint =
      FindPredefinedType(result.name()).enum_value();

  size_t size = blueprint.enumerator_size();
  size_t idx = rand_(size);

  ScalarDataValueMessage scalar_value = blueprint.scalar_value(idx);
  string scalar_type = blueprint.scalar_type();

  // Mutate this enum like a scalar with probability
  // odds_for/(odds_for + odds_against).
  uint64_t odds_for = (mutator_->mutator_bias_).enum_bias_.first;
  uint64_t odds_against = (mutator_->mutator_bias_).enum_bias_.second;
  uint64_t rand_num = rand_(odds_for + odds_against);
  if (rand_num < odds_for) {
    scalar_value = ProtoFuzzerScalarMutator::Mutate(scalar_value, scalar_type);
  }

  *(result.mutable_scalar_value()) = scalar_value;
  result.set_scalar_type(scalar_type);
  return result;
}

VariableSpecificationMessage ProtoFuzzerEnumMutator::Mutate(
    const VariableSpecificationMessage &var_spec) {
  return RandomGen(var_spec);
}

}  // namespace vts
}  // namespace android
