#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := vts_proto_fuzzer_prototype
LOCAL_SRC_FILES := \
    ProtoFuzzerPrototype.cpp \

LOCAL_C_INCLUDES := \
    external/llvm/lib/Fuzzer \
    test/vts/sysfuzzer/common/include \
    test/vts-testcase/fuzz/proto_fuzzer/include \

LOCAL_SHARED_LIBRARIES := \
    libprotobuf-cpp-full \
    libvts_multidevice_proto \
    libvts_common \
    libvts_proto_fuzzer \
    libvts_proto_fuzzer_proto \
    android.hardware.light@2.0 \
    android.hardware.light.vts.driver@2.0 \

LOCAL_STATIC_LIBRARIES := \
    libLLVMFuzzer \

LOCAL_CFLAGS := \
    -Wno-unused-parameter \
    -Wno-macro-redefined \
    -Wno-extra-tokens \
    -Wno-c99-extensions \
    -fno-omit-frame-pointer \

LOCAL_SANITIZE := address coverage

include $(BUILD_EXECUTABLE)
