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

#include "ProtoFuzzerTest.h"
#include "type_mutators/ProtoFuzzerScalarMutator.h"

#include <gtest/gtest.h>

#include <iostream>

using ::std::cout;
using ::android::vts::FakeRandom;
using ::android::vts::ProtoFuzzerScalarMutator;
using ::android::vts::VariableSpecificationMessage;
using ::android::vts::ScalarDataValueMessage;
using ::std::make_unique;
using ::std::unique_ptr;
using ::std::unordered_map;

class ProtoFuzzerScalarMutatorTest : public ::testing::Test {
 public:
  virtual void SetUp() override {
    unordered_map<string, VariableSpecificationMessage> predefined_types{};
    mutator_ =
        make_unique<ProtoFuzzerScalarMutator>(rand_, nullptr, predefined_types);
  }

  FakeRandom rand_;
  unique_ptr<ProtoFuzzerScalarMutator> mutator_;
};

// Test ProtoFuzzerScalarMutator::RandomGen() methods.
TEST_F(ProtoFuzzerScalarMutatorTest, RandomGenTest) {
  // Assume little-endianness.
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "bool_t");
    EXPECT_FALSE(value.bool_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "int8_t");
    EXPECT_EQ(0xbeef0000, value.int8_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "uint8_t");
    EXPECT_EQ(0xbeef0000, value.uint8_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "int16_t");
    EXPECT_EQ(0xbeef0000, value.int16_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "uint16_t");
    EXPECT_EQ(0xbeef0000, value.uint16_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "int32_t");
    EXPECT_EQ(0xbeef0000, value.int32_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "uint32_t");
    EXPECT_EQ(0xbeef0000, value.uint32_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "int64_t");
    EXPECT_EQ(0xffffdeadbeef0000, value.int64_t());
  }
  {
    ScalarDataValueMessage value = mutator_->RandomGen(value, "uint64_t");
    EXPECT_EQ(0xffffdeadbeef0000, value.uint64_t());
  }
}

// Test ProtoFuzzerScalarMutator::Mutate() methods.
TEST_F(ProtoFuzzerScalarMutatorTest, MutateTest) {
  {
    ScalarDataValueMessage value{};
    value.set_bool_t(0xdeadbeef);
    EXPECT_FALSE((mutator_->Mutate(value, "bool_t")).bool_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_int8_t(0xdeadbeef);
    EXPECT_EQ(0x9eadbeef, (mutator_->Mutate(value, "int8_t")).int8_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_uint8_t(0xdeadbeef);
    EXPECT_EQ(0x9eadbeef, (mutator_->Mutate(value, "uint8_t")).uint8_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_int16_t(0xdeadbeef);
    EXPECT_EQ(0x9eadbeef, (mutator_->Mutate(value, "int16_t")).int16_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_uint16_t(0xdeadbeef);
    EXPECT_EQ(0x9eadbeef, (mutator_->Mutate(value, "uint16_t")).uint16_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_int32_t(0xdeadbeef);
    EXPECT_EQ(0x9eadbeef, (mutator_->Mutate(value, "int32_t")).int32_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_uint32_t(0xdeadbeef);
    EXPECT_EQ(0x9eadbeef, (mutator_->Mutate(value, "uint32_t")).uint32_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_int64_t(0xffffdeadbeef0000);
    EXPECT_EQ(0xbfffdeadbeef0000,
              (mutator_->Mutate(value, "int64_t")).int64_t());
  }
  {
    ScalarDataValueMessage value{};
    value.set_uint64_t(0xffffdeadbeef0000);
    EXPECT_EQ(0xbfffdeadbeef0000,
              (mutator_->Mutate(value, "uint64_t")).uint64_t());
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
