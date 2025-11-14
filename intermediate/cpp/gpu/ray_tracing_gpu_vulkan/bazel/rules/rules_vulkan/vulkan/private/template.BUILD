load("@bazel_skylib//rules:native_binary.bzl", "native_binary")
load("@rules_cc//cc:cc_import.bzl", "cc_import")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_vulkan//vulkan:toolchains.bzl", "vulkan_toolchain")

package(default_visibility = ["//visibility:public"])

# Export SDK files to allow external wrappers.
exports_files(glob(
    [
        "sdk/bin/**",
        "sdk/Bin/**",
    ],
    allow_empty = True,
))

filegroup(
    name = "headers",
    srcs = glob([
        "{include_path}/**/*.h",
        "{include_path}/**/*.hpp",
    ]),
)

cc_import(
    name = "vulkan_dll",
    shared_library = "vulkan-1.dll",
    target_compatible_with = [
        "@platforms//os:windows",
    ],
)

# This uses workaround from https://github.com/bazelbuild/bazel/issues/4748
cc_library(
    name = "vulkan",
    # buildifier: disable=constant-glob
    srcs = glob(["{lib_vulkan}"]),
    hdrs = [":headers"],
    includes = ["{include_path}"],
    deps = [{vulkan_deps}],
)

#
# SDK compilers and tools
#

native_binary(
    name = "dxc",
    src = select({
        "@platforms//os:windows": "sdk/Bin/dxc.exe",
        "//conditions:default": "sdk/bin/dxc",
    }),
    env = select({
        "@platforms//os:windows": {
            "PATH": "{sdk_root}/Bin",
        },
        "//conditions:default": {},
    }),
)

native_binary(
    name = "glslc",
    src = select({
        "@platforms//os:windows": "sdk/Bin/glslc.exe",
        "//conditions:default": "sdk/bin/glslc",
    }),
)

native_binary(
    name = "slangc",
    src = select({
        "@platforms//os:windows": "sdk/Bin/slangc.exe",
        "//conditions:default": "sdk/bin/slangc",
    }),
    env = select({
        "@platforms//os:windows": {
            "PATH": "{sdk_root}/Bin",
        },
        "@platforms//os:linux": {
            "LD_LIBRARY_PATH": "{sdk_root}/lib",
        },
        "//conditions:default": {},
    }),
)

native_binary(
    name = "spirv_cross",
    src = select({
        "@platforms//os:windows": "sdk/Bin/spirv-cross.exe",
        "//conditions:default": "sdk/bin/spirv-cross",
    }),
)

#
# Vulkan toolchain
#

vulkan_toolchain(
    name = "vulkan_sdk_{os}",
    dxc = ":dxc",
    env = select({
        # Required by dxc and slangc on Windows
        "@platforms//os:windows": {
            "PATH": "{sdk_root}/Bin",
        },
        # Required by slangc on Linux
        "@platforms//os:linux": {
            "LD_LIBRARY_PATH": "{sdk_root}/lib",
        },
        "//conditions:default": {},
    }),
    glslc = ":glslc",
    slangc = ":slangc",
    spirv_cross = ":spirv_cross",
)

toolchain(
    name = "vulkan_sdk_{os}_toolchain",
    exec_compatible_with = [
        "@platforms//os:{os}",
    ],
    toolchain = ":vulkan_sdk_{os}",
    toolchain_type = "@rules_vulkan//vulkan:toolchain_type",
)
