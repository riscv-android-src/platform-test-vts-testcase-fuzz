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

#ifndef __VTS_PROTO_FUZZER_SCALAR_MUTATOR_H__
#define __VTS_PROTO_FUZZER_SCALAR_MUTATOR_H__

#include "test/vts/proto/ComponentSpecificationMessage.pb.h"
#include "type_mutators/ProtoFuzzerTypeMutator.h"

using std::string;
using std::unordered_map;

namespace android {
namespace vts {

// Mutates/random generates VariableSpecificationMessage of TYPE_SCALAR.
class ProtoFuzzerScalarMutator : public ProtoFuzzerTypeMutator {
 public:
  ProtoFuzzerScalarMutator(
      Random &rand, ProtoFuzzerMutator *mutator,
      const unordered_map<string, VariableSpecificationMessage>
          &predefined_types)
      : ProtoFuzzerTypeMutator(rand, mutator, predefined_types) {}

  VariableSpecificationMessage RandomGen(
      const VariableSpecificationMessage &var_spec) override;
  VariableSpecificationMessage Mutate(
      const VariableSpecificationMessage &var_spec) override;

  // Generates a random ScalarDataValueMessage.
  ScalarDataValueMessage RandomGen(const ScalarDataValueMessage &scalar_value,
                                   const string &scalar_type);
  // Mutates a ScalarDataValueMessage.
  ScalarDataValueMessage Mutate(const ScalarDataValueMessage &scalar_value,
                                const string &scalar_type);

 private:
  // Methods to generate a random value of primitive type.
  template <typename T>
  T RandomGen(T value);
  bool RandomGen(bool value);

  // Methods to mutate value of primitive type.
  template <typename T>
  T Mutate(T value);
  bool Mutate(bool value);
  float Mutate(float value);
  double Mutate(double value);
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_PROTO_FUZZER_SCALAR_MUTATOR_H__
