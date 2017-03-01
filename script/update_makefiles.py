#
# Copyright (C) 2017 The Android Open Source Project
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

from build.func_fuzzer_build_rule_gen import FuncFuzzerBuildRuleGen
import os
import sys

if __name__ == '__main__':
    usage = (
    'Updates .bp and .mk files under test/vts-testcase/fuzz.\n'
    'Among .bp and .mk files affected are:\n'
    '1. test/vts-testcase/fuzz/Android.bp\n'
    '2. files matching: test/vts-testcase/fuzz/func_fuzzer/Android.bp\n\n'
    '3. files matching: test/vts-testcase/fuzz/func_fuzzer/<fuzz_name>/<fuzz_version>/Android.bp\n\n'
    'Usage:\n'
    '  cd test/vts-testcase/fuzz && python update_makefiles.py\n')

    print 'Updating build rules.'
    build_rule_gen = FuncFuzzerBuildRuleGen()
    build_rule_gen.UpdateBuildRule()
