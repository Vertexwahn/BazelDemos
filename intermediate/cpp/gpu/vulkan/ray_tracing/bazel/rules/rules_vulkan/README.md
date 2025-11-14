<p align="center">
  <img src="docs/logo.png" />
</p>

# rules_vulkan

[![CI](https://github.com/mxpv/rules_vulkan/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mxpv/rules_vulkan/actions/workflows/ci.yml)
[![License](https://img.shields.io/github/license/mxpv/rules_vulkan)](./LICENSE)
![Release](https://img.shields.io/github/v/release/mxpv/rules_vulkan)

`rules_vulkan` is a set of [Bazel](https://bazel.build) rules for integrating the [Vulkan SDK](https://vulkan.lunarg.com/)
into your builds. It streamlines downloading, installing, and using Vulkan SDKs across major platforms.

- [Documentation](./docs/index.md)
  + [glsl_shader](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#glsl_shader)
  + [hlsl_shader](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#hlsl_shader)
  + [slang_shader](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#slang_shader)
  + [spirv_cross](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#spirv_cross)
  + [shader_group](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#shader_group)
  + [download_sdk](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#download_sdk)
- [Download SDKs](https://vulkan.lunarg.com)

## Features
- Fully automated SDK installation.
- Maintains a list of [currently available](./vulkan/private/versions.bzl) SDK versions on `LunarG` with automated [nightly updates](https://github.com/mxpv/rules_vulkan/actions/workflows/update.yml).
- Toolchains and rules for [`GLSL`](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#glsl_shader), [`HLSL`](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#hlsl_shader), and [`Slang`](https://github.com/mxpv/rules_vulkan/blob/main/docs/index.md#slang_shader).
- Unit and integration tests on CI.
- Available on [BCR](https://registry.bazel.build/modules/rules_vulkan).
- A nice-looking AI-generated logo!


## :beginner: Getting started

To get started, you’ll need to fetch the Vulkan SDK and register the toolchains.
Add the following to your `MODULE.bazel` file:

```bazel
bazel_dep(name = "rules_vulkan", version = "0.1")

vulkan_sdk = use_extension("@rules_vulkan//vulkan:extensions.bzl", "vulkan_sdk")

vulkan_sdk.toolchain(version = "1.4.313")
use_repo(vulkan_sdk, "vulkan_sdk_1.4.313")

register_toolchains("@vulkan_sdk_1.4.313//:all")
```

Then use it in your `BUILD` files:

```bazel
load("@rules_vulkan//vulkan:defs.bzl", "hlsl_shader", "glsl_shader")

hlsl_shader(
    name = "hello_hlsl",
    src = "shader.hlsl",
    entry = "CSMain",
    target = "cs_6_0",
    spirv = True,
    hdrs = [":common_headers"],
)

glsl_shader(
    name = "hello_glsl",
    src = "shader.glsl",
    stage = "comp",
)

cc_binary(
    name = "app",
    srcs = ["main.c"],
    data = [":hello_hlsl", ":hello_glsl"],
    deps = ["@vulkan_sdk_1.4.313//:vulkan"],
)

# Since the SDK repo exports all binaries, you can manually wrap any SDK binary not directly exposed
native_binary(
    name = "spirv_cross",
    src = select({
        "@platforms//os:windows": "@vulkan_sdk_1.4.313//:sdk/Bin/spirv-cross.exe",
        "//conditions:default": "@vulkan_sdk_1.4.313//:sdk/bin/spirv-cross",
    }),
)

```

### ⚡ Direct Binary Invocation

You can invoke SDK binaries directly using their pre-configured targets:

```bash
# Run spirv-cross directly
bazelisk run @vk_sdk//:spirv_cross -- --help

# Run glslc directly
bazelisk run @vk_sdk//:glslc -- --version

# Run dxc directly
bazelisk run @vk_sdk//:dxc -- --version

# Run slangc directly
bazelisk run @vk_sdk//:slangc -- -h
```

Refer to `e2e` project [here](./e2e/smoke/BUILD) for a more complete setup.

## License

The project itself is licensed under [`Apache 2.0`](./LICENSE) license.

> [!NOTE]
> This project downloads packages from LunarG, please ensure you comply with their license terms.

