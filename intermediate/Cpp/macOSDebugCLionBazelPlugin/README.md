<!--
SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Notes

This demo tries to get debugging working on the following system:

- M1 processor
- macOS 12.5.1 
- CLion 2022.2.3
- Bazel Plugin 2022.08.24.0.1-api-version-222
- Bazel 5.3.0
- Apple clang version 13.1.6 (clang-1316.0.21.2.5)

# How to get debugging working?

Create a `.lldbinit` file in your home directory with the following content:

```
settings set target.source-map . <path_to_your_workspace>
```

# References

- https://github.com/bazelbuild/intellij/issues/494
