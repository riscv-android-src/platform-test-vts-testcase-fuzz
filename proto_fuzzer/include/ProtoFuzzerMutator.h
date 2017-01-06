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

#ifndef __VTS_PROTO_FUZZER_MUTATOR_H_
#define __VTS_PROTO_FUZZER_MUTATOR_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "ProtoFuzzerUtils.h"
#include "test/vts-testcase/fuzz/proto_fuzzer/proto/ExecutionSpecificationMessage.pb.h"
#include "test/vts/proto/ComponentSpecificationMessage.pb.h"
#include "type_mutators/ProtoFuzzerScalarMutator.h"

using std::string;
using std::unique_ptr;
using std::unordered_map;

namespace android {
namespace vts {

// Provides methods to mutate or randomly generate
// ExecutionSpecificationMessage, FunctionSpecificationMessage, or
// VariableSpecificationMessage.
class ProtoFuzzerMutator {
 public:
  ProtoFuzzerMutator(
      Random &rand,
      unordered_map<string, VariableSpecificationMessage> predefined_types);

  // Generates a random ExecutionSpecificationMessage.
  ExecutionSpecificationMessage RandomGen(
      const InterfaceSpecificationMessage &iface_spec, size_t num_calls);
  // Mutates in-place an ExecutionSpecificationMessage.
  void Mutate(const InterfaceSpecificationMessage &iface_spec,
              ExecutionSpecificationMessage *exec_spec);

  // Generates a random FunctionSpecificationMessage.
  FunctionSpecificationMessage RandomGen(
      const FunctionSpecificationMessage &func_spec);
  // Mutates a FunctionSpecificationMessage.
  FunctionSpecificationMessage Mutate(
      const FunctionSpecificationMessage &func_spec);

  // Generates a random VariableSpecificationMessage.
  VariableSpecificationMessage RandomGen(
      const VariableSpecificationMessage &var_spec);
  // Mutates a VariableSpecificationMessage.
  VariableSpecificationMessage Mutate(
      const VariableSpecificationMessage &var_spec);

 private:
  // 64-bit random number generator.
  Random &rand_;
  // Used to look up definition of a predefined type by its name.
  unordered_map<string, VariableSpecificationMessage> predefined_types_;
  // Used to delegate mutation/random generation of VariableSpecifationMessages
  // of different VariableType to mutator for that VariableType.
  unordered_map<VariableType, unique_ptr<ProtoFuzzerTypeMutator>>
      type_mutators_;
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_PROTO_FUZZER_MUTATOR__
