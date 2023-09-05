# Readme

How to build?

```shell
bazel build //...
```

How to run?

```shell
bazel run //:run_glad -- --help
bazel run //:run_glad -- --api gl:core=3.3 --out-path tmp
```
