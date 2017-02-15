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

#ifndef __VTS_PROTO_FUZZER_HANDLE_MUTATOR_H__
#define __VTS_PROTO_FUZZER_HANDLE_MUTATOR_H__

#include "test/vts/proto/ComponentSpecificationMessage.pb.h"
#include "type_mutators/ProtoFuzzerTypeMutator.h"

#include "ProtoFuzzerMutator.h"

using std::string;
using std::unordered_map;

namespace android {
namespace vts {

// Mutates/random generates VariableSpecificationMessage of TYPE_HANDLE.
class ProtoFuzzerHandleMutator : public ProtoFuzzerTypeMutator {
 public:
  ProtoFuzzerHandleMutator(
      Random &rand, ProtoFuzzerMutator *mutator,
      const unordered_map<string, VariableSpecificationMessage>
          &predefined_types)
      : ProtoFuzzerTypeMutator(rand, mutator, predefined_types) {}

  // TODO(trong): once vts driver suports TYPE_HANDLE, these will need
  // implementation.
  VariableSpecificationMessage RandomGen(
      const VariableSpecificationMessage &var_spec) override {
    return VariableSpecificationMessage{var_spec};
  }

  VariableSpecificationMessage Mutate(
      const VariableSpecificationMessage &var_spec) override {
    return VariableSpecificationMessage{var_spec};
  }
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_PROTO_FUZZER_HANDLE_MUTATOR_H__
