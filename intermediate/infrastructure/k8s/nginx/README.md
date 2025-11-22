<!--
SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Notes

```shell
minikube start
kubectl get namespaces
kubectl get pods
# list all targets
bazel query //...
bazel run //:nginx.create # make sure cluster `minikube` is in place
# do some changes on deployment (change replicas to 2)
bazel run //:nginx.replace
```
