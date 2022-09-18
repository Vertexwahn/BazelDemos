<!--
SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Coverage

```shell
bazelisk coverage -s \                                         
  --instrument_test_targets \
  --experimental_cc_coverage \
  --combined_report=lcov \
  --coverage_report_generator=@bazel_tools//tools/test/CoverageOutputGenerator/java/com/google/devtools/coverageoutputgenerator:Main \
  //...
```

HTML Output:

```shell
genhtml /home/newtuseron/.cache/bazel/_bazel_newton/e74a825d0eb0c2db4b6f8afd819b274c/execroot/__main__/bazel-out/k8-fastbuild/testlogs/foo_test/coverage.dat --output-directory coverage
```
