<!--
SPDX-FileCopyrightText: 2024-2025 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Notes

## How to run?

Run hello_world application:

```shell
bazel run //:hello_world
```

## How to recreate?

Create files for a minimal C++ Bazel demo:

```shell
mkdir cpp_hello_world

cd cpp_hello_world

cat << EOF > MODULE.bazel
bazel_dep(name = "rules_cc", version = "0.1.1")
EOF

cat << EOF > main.cpp
#include <iostream>

int main(int argc, char const *argv[]) {
    std::cout << "Hello World!" << std::endl;
}
EOF

cat << EOF > BUILD.bazel
load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "hello_world",
    srcs = ["main.cpp"],
)
EOF

bazel run //:hello_world
```
