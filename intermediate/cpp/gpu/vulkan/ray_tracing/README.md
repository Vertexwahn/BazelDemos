# Ray Tracing

This was copied from https://github.com/TwentyFiveSoftware/ray-tracing-gpu-vulkan.
The difference to the original one is, that this vesion uses Bazel as build sytem.

![](sceneRender.png)

## Run

*Ubuntu 24.04*

```shell
bazel run --config=gcc13 //:RayTracingGPUVulkan
```

*Windows with Visual Studio 2022*

```shell
bazel run --config=vs2022 //:RayTracingGPUVulkan
```
