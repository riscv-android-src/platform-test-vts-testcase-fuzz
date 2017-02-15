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

#ifndef __VTS_PROTO_FUZZER_ENUM_MUTATOR_H__
#define __VTS_PROTO_FUZZER_ENUM_MUTATOR_H__

#include "ProtoFuzzerMutator.h"
#include "test/vts/proto/ComponentSpecificationMessage.pb.h"
#include "type_mutators/ProtoFuzzerScalarMutator.h"

namespace android {
namespace vts {

// Mutates/random generates VariableSpecificationMessage of TYPE_ENUM.
class ProtoFuzzerEnumMutator : public ProtoFuzzerScalarMutator {
 public:
  ProtoFuzzerEnumMutator(
      Random &rand, ProtoFuzzerMutator *mutator,
      const unordered_map<string, VariableSpecificationMessage>
          &predefined_types)
      : ProtoFuzzerScalarMutator(rand, mutator, predefined_types) {}

  VariableSpecificationMessage RandomGen(
      const VariableSpecificationMessage &var_spec) override;

  VariableSpecificationMessage Mutate(
      const VariableSpecificationMessage &var_spec) override;
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_PROTO_FUZZER_ENUM_MUTATOR_H__
