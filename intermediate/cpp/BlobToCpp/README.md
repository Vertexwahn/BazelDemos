<!--
SPDX-FileCopyrightText: 2023 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Notes

Build application:

```shell
bazel build //:Demo
```

# Generating C++ files via py_binary and genrule

I have a Python script named `blob_to_cpp.py` (located at `scirpts/blob_to_cpp.py` relative to the `WORKSPACE.bazel` file). The Python script takes an input file (e.g. `weights/rt_alb.tza`) and generates form that a C++ header (`.h`) and source file (`.cpp`) that I want to add to a `cc_binary`.

The source code of my minimal reproducible example can be found [here](https://github.com/Vertexwahn/BazelDemos/tree/main/intermediate/Cpp/BlobToCpp).

The Python script can be called via:

```shell
bazel run //:blob_to_cpp -- -o weights/rt_alb.cpp -H weights/rt_alb.h weights/rt_alb.tza
```

I try to use a `genrule` to invoke the python script (bazelized via `py_binary` as `//:blob_to_cpp`)

*bazel/odin_generate_cpp_from_blob.bzl:*
```python
def generate_cpp_from_blob_cc_library(name, **kwargs):
    native.genrule(
        name = "%s_weights_gen" % name,
        srcs = ["weights/" + name],
        outs = [
            "weights/" + name[0:-4] + ".cpp",
            "weights/" + name[0:-4] + ".h",
        ],
        cmd = "./$(location //:blob_to_cpp) weights/%s -o weights/%s.cpp -H weights/%s.h" % (name, name[0:-4], name[0:-4]),
        tools = ["//:blob_to_cpp"],
    )
    native.cc_library(
        name = name,
        srcs = ["weights/" + name[0:-4] + ".cpp"],
        hdrs = ["weights/" + name[0:-4] + ".h"],
        **kwargs
    )
```

When the `generate_cpp_from_blob_cc_library` Bazel macro is invoked I receive the following error messages (`bazel build //:Demo`):

```
ERROR: /Users/vertexwahn/dev/Piper/BazelDemos/intermediate/Cpp/BlobToCpp/BUILD.bazel:14:34: declared output 'weights/rt_alb.cpp' was not created by genrule. This is probably because the genrule actually didn't create this output, or because the output was a directory and the genrule was run remotely (note that only the contents of declared file outputs are copied from genrules run remotely)
ERROR: /Users/vertexwahn/dev/Piper/BazelDemos/intermediate/Cpp/BlobToCpp/BUILD.bazel:14:34: declared output 'weights/rt_alb.h' was not created by genrule. This is probably because the genrule actually didn't create this output, or because the output was a directory and the genrule was run remotely (note that only the contents of declared file outputs are copied from genrules run remotely)
ERROR: /Users/vertexwahn/dev/Piper/BazelDemos/intermediate/Cpp/BlobToCpp/BUILD.bazel:14:34: Executing genrule //:rt_alb.tza_weights_gen failed: not all outputs were created or valid
Target //:Demo failed to build
```

My goal is to generate the files `weights/rt_alb.cpp` and `weights/rt_alb.h`. I need them in the `weights` folder since my `cc_binary` is expecting that the header file is within the `weights` folder (`#include "weights/rt_alb.h"`).

My `BUILD.bazel` file looks like this:

```
load("//bazel:odin_generte_cpp_from_blob.bzl", "generate_cpp_from_blob_cc_library")

py_binary(
    name = "blob_to_cpp",
    srcs = ["scripts/blob_to_cpp.py"],
    data = ["weights/rt_alb.tza"]
)

generate_cpp_from_blob_cc_library(
    name = "rt_alb.tza"
)

cc_binary(
    name = "Demo",
    srcs = ["main.cpp"],
    deps = [":rt_alb.tza"],
)
```

Any hints to get this working are welcome!

Tags: bazel bazel-python bazel-cpp bazel-genrule
