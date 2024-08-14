# Readme

Install the following on Ubuntu 22.04:

$ sudo apt install libncurses5

```shell
bazel run //:hello_world
bazel run --toolchain_resolution_debug=".*" //:hello_world

bazel run --platforms=@local_config_platform//:host //:hello_world


bazel run --platforms=@local_config_platform//:host --noincompatible_enable_cc_toolchain_resolution //:hello_world
```

## References

- https://stackoverflow.com/questions/78117798/llvm-and-gcc-toolchain-selection
