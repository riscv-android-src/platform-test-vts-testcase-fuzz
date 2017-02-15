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

#ifndef __VTS_PROTO_FUZZER_TYPE_MUTATOR_H__
#define __VTS_PROTO_FUZZER_TYPE_MUTATOR_H__

#include <string>
#include <unordered_map>

#include "ProtoFuzzerUtils.h"
#include "test/vts/proto/ComponentSpecificationMessage.pb.h"

using std::string;
using std::unordered_map;

namespace android {
namespace vts {

class ProtoFuzzerMutator;

// Base class for type-specific mutators.
class ProtoFuzzerTypeMutator {
 public:
  ProtoFuzzerTypeMutator(
      Random &rand, ProtoFuzzerMutator *mutator,
      const unordered_map<string, VariableSpecificationMessage>
          &predefined_types)
      : rand_(rand), mutator_(mutator), predefined_types_(predefined_types) {}

  virtual ~ProtoFuzzerTypeMutator() {}

  // Generates a random VariableSpecificationMessage.
  virtual VariableSpecificationMessage RandomGen(
      const VariableSpecificationMessage &scalar_value) = 0;
  // Mutates a VariableSpecificationMessage.
  virtual VariableSpecificationMessage Mutate(
      const VariableSpecificationMessage &scalar_value) = 0;

 protected:
  // Looks up predefined type by name.
  const VariableSpecificationMessage &FindPredefinedType(string name);
  // 64-bit random number generator.
  Random &rand_;
  // Mutates/random generates a nested VariableSpecificationMessage.
  ProtoFuzzerMutator *mutator_;
  // Used to look up definition of a predefined type by its name.
  const unordered_map<string, VariableSpecificationMessage> &predefined_types_;
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_PROTO_FUZZER_TYPE_MUTATOR_H__
