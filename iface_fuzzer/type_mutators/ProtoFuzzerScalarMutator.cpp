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

#include "type_mutators/ProtoFuzzerScalarMutator.h"

using std::memcpy;
using std::string;

namespace android {
namespace vts {

VariableSpecificationMessage ProtoFuzzerScalarMutator::RandomGen(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{var_spec};
  (*result.mutable_scalar_value()) =
      RandomGen(var_spec.scalar_value(), var_spec.scalar_type());

  return result;
}

VariableSpecificationMessage ProtoFuzzerScalarMutator::Mutate(
    const VariableSpecificationMessage &var_spec) {
  VariableSpecificationMessage result{var_spec};
  (*result.mutable_scalar_value()) =
      Mutate(var_spec.scalar_value(), var_spec.scalar_type());

  return result;
}

ScalarDataValueMessage ProtoFuzzerScalarMutator::RandomGen(
    const ScalarDataValueMessage &scalar_value, const string &scalar_type) {
  ScalarDataValueMessage result;

  if (scalar_type == "bool_t") {
    result.set_bool_t(RandomGen(static_cast<bool>(scalar_value.bool_t())));
  } else if (scalar_type == "int8_t") {
    result.set_int8_t(RandomGen(scalar_value.int8_t()));
  } else if (scalar_type == "uint8_t") {
    result.set_uint8_t(RandomGen(scalar_value.uint8_t()));
  } else if (scalar_type == "int16_t") {
    result.set_int16_t(RandomGen(scalar_value.int16_t()));
  } else if (scalar_type == "uint16_t") {
    result.set_uint16_t(RandomGen(scalar_value.uint16_t()));
  } else if (scalar_type == "int32_t") {
    result.set_int32_t(RandomGen(scalar_value.int32_t()));
  } else if (scalar_type == "uint32_t") {
    result.set_uint32_t(RandomGen(scalar_value.uint32_t()));
  } else if (scalar_type == "int64_t") {
    result.set_int64_t(RandomGen(scalar_value.int64_t()));
  } else if (scalar_type == "uint64_t") {
    result.set_uint64_t(RandomGen(scalar_value.uint64_t()));
  } else if (scalar_type == "float_t") {
    result.set_float_t(RandomGen(scalar_value.float_t()));
  } else if (scalar_type == "double_t") {
    result.set_double_t(RandomGen(scalar_value.double_t()));
  } else {
    cout << scalar_type << " not supported.\n";
  }

  return result;
}

ScalarDataValueMessage ProtoFuzzerScalarMutator::Mutate(
    const ScalarDataValueMessage &scalar_value, const string &scalar_type) {
  ScalarDataValueMessage result;

  if (scalar_type == "bool_t") {
    result.set_bool_t(Mutate(static_cast<bool>(scalar_value.bool_t())));
  } else if (scalar_type == "int8_t") {
    result.set_int8_t(Mutate(scalar_value.int8_t()));
  } else if (scalar_type == "uint8_t") {
    result.set_uint8_t(Mutate(scalar_value.uint8_t()));
  } else if (scalar_type == "int16_t") {
    result.set_int16_t(Mutate(scalar_value.int16_t()));
  } else if (scalar_type == "uint16_t") {
    result.set_uint16_t(Mutate(scalar_value.uint16_t()));
  } else if (scalar_type == "int32_t") {
    result.set_int32_t(Mutate(scalar_value.int32_t()));
  } else if (scalar_type == "uint32_t") {
    result.set_uint32_t(Mutate(scalar_value.uint32_t()));
  } else if (scalar_type == "int64_t") {
    result.set_int64_t(Mutate(scalar_value.int64_t()));
  } else if (scalar_type == "uint64_t") {
    result.set_uint64_t(Mutate(scalar_value.uint64_t()));
  } else if (scalar_type == "float_t") {
    result.set_float_t(Mutate(scalar_value.float_t()));
  } else if (scalar_type == "double_t") {
    result.set_double_t(Mutate(scalar_value.double_t()));
  } else {
    cout << scalar_type << " not supported.\n";
  }

  return result;
}

template <typename T>
T ProtoFuzzerScalarMutator::RandomGen(T value) {
  uint64_t rand_int = rand_.Rand();
  T result;
  memcpy(&result, &rand_int, sizeof(T));
  return result;
}

bool ProtoFuzzerScalarMutator::RandomGen(bool value) {
  return static_cast<bool>(rand_(2));
}

template <typename T>
T ProtoFuzzerScalarMutator::Mutate(T value) {
  size_t num_bits = 8 * sizeof(T);
  T mask = static_cast<T>(1) << rand_(num_bits);
  return value ^ mask;
}

bool ProtoFuzzerScalarMutator::Mutate(bool value) { return rand_(2); }

float ProtoFuzzerScalarMutator::Mutate(float value) {
  uint32_t copy;
  memcpy(&copy, &value, sizeof(float));
  uint32_t mask = static_cast<uint32_t>(1) << rand_(32);
  copy ^= mask;
  memcpy(&value, &copy, sizeof(float));
  return value;
}

double ProtoFuzzerScalarMutator::Mutate(double value) {
  uint64_t copy;
  memcpy(&copy, &value, sizeof(double));
  uint64_t mask = static_cast<uint64_t>(1) << rand_(32);
  copy ^= mask;
  memcpy(&value, &copy, sizeof(double));
  return value;
}

}  // namespace vts
}  // namespace android
