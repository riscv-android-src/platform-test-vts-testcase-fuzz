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
LOCAL_MODULE := libvts_proto_fuzzer
LOCAL_SRC_FILES := \
    ProtoFuzzerUtils.cpp \
    ProtoFuzzerMutator.cpp \
    type_mutators/ProtoFuzzerEnumMutator.cpp \
    type_mutators/ProtoFuzzerScalarMutator.cpp \
    type_mutators/ProtoFuzzerStructMutator.cpp \

LOCAL_C_INCLUDES := \
    test/vts/sysfuzzer/common/include \
    test/vts-testcase/fuzz/proto_fuzzer/include \

LOCAL_SHARED_LIBRARIES := \
    libprotobuf-cpp-full \
    libvts_multidevice_proto \
    libvts_proto_fuzzer_proto \

LOCAL_CFLAGS := \
    -Wno-unused-parameter \

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))
