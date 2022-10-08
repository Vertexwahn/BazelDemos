<!--
SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Notes

```shell
# list all targets
bazel query //...
bazel run //:nginx.create # make sure cluster `minikube` is in place
# do some changes on deployment
bazel run //:nginx.replace
```
