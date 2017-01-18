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

#include "type_mutators/ProtoFuzzerArrayMutator.h"

namespace android {
namespace vts {

VariableSpecificationMessage ProtoFuzzerArrayMutator::RandomGen(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{};
  result.set_type(TYPE_ARRAY);
  int vector_size = var_spec.vector_value(0).vector_size();
  result.set_vector_size(vector_size);
  for (int i = 0; i < vector_size; ++i) {
    *result.add_vector_value() = mutator_->RandomGen(var_spec.vector_value(0));
  }
  return result;
}

VariableSpecificationMessage ProtoFuzzerArrayMutator::Mutate(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{var_spec};
  size_t vector_size = static_cast<size_t>(var_spec.vector_size());
  size_t idx = rand_(vector_size);
  *result.mutable_vector_value(idx) =
      mutator_->Mutate(var_spec.vector_value(idx));
  return result;
}

}  // namespace vts
}  // namespace android
