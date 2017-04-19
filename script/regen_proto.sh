#!/bin/bash
#
# Copyright 2017 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if [ -z "$ANDROID_BUILD_TOP" ]; then
    echo "Missing ANDROID_BUILD_TOP env variable. Run 'lunch' first."
    exit 1
fi

IFACE_FUZZER_PROTO_DIR="iface_fuzzer/proto/"

## Modifies import statement in proto files.
sed -i 's?test/vts/proto/?vts/proto/?g' $IFACE_FUZZER_PROTO_DIR/ExecutionSpecificationMessage.proto

protoc -I=$ANDROID_BUILD_TOP/test:$IFACE_FUZZER_PROTO_DIR --python_out=$IFACE_FUZZER_PROTO_DIR $IFACE_FUZZER_PROTO_DIR/ExecutionSpecificationMessage.proto

## Restores import statement in proto files.
sed -i 's?vts/proto/?test/vts/proto/?g' $IFACE_FUZZER_PROTO_DIR/ExecutionSpecificationMessage.proto
