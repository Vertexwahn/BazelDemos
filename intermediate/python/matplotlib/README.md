<!--
SPDX-FileCopyrightText: 2025 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Notes

## How to run?

```shell
bazel run //:demo
```

## Update dependencies

```shell
bazel run //:requirements.update
```

## List all dependencies

```shell
bazel query @pypi//...
```
