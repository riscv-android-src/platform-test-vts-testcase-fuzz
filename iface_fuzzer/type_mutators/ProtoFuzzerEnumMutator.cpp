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

#include <cstring>
#include <iostream>

using std::cout;
using std::endl;
using std::memcpy;
using std::string;

namespace android {
namespace vts {

VariableSpecificationMessage ProtoFuzzerEnumMutator::RandomGen(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result;
  result.set_name(var_spec.predefined_type());
  result.set_type(TYPE_ENUM);

  const EnumDataValueMessage &blueprint =
      predefined_types_.at(result.name()).enum_value();

  size_t size = blueprint.enumerator_size();
  size_t idx = rand_(size);

  EnumDataValueMessage *enum_value = result.mutable_enum_value();

  enum_value->add_enumerator(blueprint.enumerator(idx));
  *(enum_value->add_scalar_value()) = blueprint.scalar_value(idx);
  enum_value->set_scalar_type(blueprint.scalar_type());

  return result;
}

VariableSpecificationMessage ProtoFuzzerEnumMutator::Mutate(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result;
  result.set_name(var_spec.name());
  result.set_type(TYPE_ENUM);

  const EnumDataValueMessage &blueprint =
      predefined_types_.at(result.name()).enum_value();

  size_t size = blueprint.enumerator_size();
  size_t idx = rand_(size);

  EnumDataValueMessage *enum_value = result.mutable_enum_value();

  enum_value->add_enumerator(blueprint.enumerator(idx));
  *(enum_value->add_scalar_value()) = blueprint.scalar_value(idx);
  enum_value->set_scalar_type(blueprint.scalar_type());

  return result;
}

}  // namespace vts
}  // namespace android
