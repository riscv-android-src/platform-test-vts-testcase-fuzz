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

#include <FuzzerInterface.h>
#include <android/hardware/light/2.0/ILight.h>
#include <android/hardware/light/2.0/Light.vts.h>
#include "ProtoFuzzerMutator.h"

#include "specification_parser/InterfaceSpecificationParser.h"
#include "test/vts/proto/ComponentSpecificationMessage.pb.h"

#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define MAX_SIZE 8192
#define NUM_CALLS 10

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;
using std::vector;

namespace android {
namespace vts {

static const char *spec_path;
static const char *types_path;

static ComponentSpecificationMessage comp_spec;
static ComponentSpecificationMessage types_spec;
static InterfaceSpecificationMessage iface_spec;
static Random random{static_cast<uint64_t>(time(0))};
static unique_ptr<ProtoFuzzerMutator> mutator;
static FuzzerBase *hal;

extern "C" int LLVMFuzzerInitialize(int *argc, char ***argv) {
  spec_path = "/data/local/tmp/proto_fuzzer/Light.vts";
  types_path = "/data/local/tmp/proto_fuzzer/types.vts";

  hal = vts_func_4_android_hardware_light_2_();
  hal->GetService(true, "light");

  InterfaceSpecificationParser::parse(spec_path, &comp_spec);
  InterfaceSpecificationParser::parse(types_path, &types_spec);
  iface_spec = comp_spec.interface();

  vector<ComponentSpecificationMessage> comp_specs{comp_spec, types_spec};
  mutator = make_unique<ProtoFuzzerMutator>(random,
                                            ExtractPredefinedTypes(comp_specs));

  return 0;
}

extern "C" size_t LLVMFuzzerCustomMutator(uint8_t *data, size_t size,
                                          size_t max_size, unsigned int seed) {
  ExecutionSpecificationMessage exec_spec;
  if (!exec_spec.ParseFromArray(data, size)) {
    exec_spec = mutator->RandomGen(iface_spec, NUM_CALLS);
  } else {
    mutator->Mutate(iface_spec, &exec_spec);
  }
  exec_spec.SerializeToArray(data, exec_spec.ByteSize());
  return exec_spec.ByteSize();
}

size_t LLVMFuzzerCustomCrossOver(const uint8_t *data1, size_t size1,
                                 const uint8_t *data2, size_t size2,
                                 uint8_t *out, size_t max_out_size,
                                 unsigned int seed) {
  memcpy(out, data1, size1);
  return size1;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  ExecutionSpecificationMessage exec_spec;
  if (!exec_spec.ParseFromArray(data, size) || exec_spec.api_size() == 0) {
    return 0;
  }
  Execute(hal, exec_spec);
  return 0;
}

}  // namespace vts
}  // namespace android
