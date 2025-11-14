load("@bazel_skylib//:bzl_library.bzl", "bzl_library")

exports_files(["defs.bzl"])

toolchain_type(
    name = "toolchain_type",
    visibility = ["//visibility:public"],
)

bzl_library(
    name = "bzl_lib",
    visibility = ["//docs:__subpackages__"],
    deps = ["//vulkan/private:bzl_lib"],
)

bzl_library(
    name = "defs",
    srcs = ["defs.bzl"],
    visibility = ["//visibility:public"],
    deps = [
        ":providers",
        ":toolchains",
        "//vulkan/private:bzl_lib",
        "//vulkan/private:download",
        "//vulkan/private:glsl",
        "//vulkan/private:hlsl",
        "//vulkan/private:shader_group",
        "//vulkan/private:slang",
        "//vulkan/private:spirv_cross",
        "//vulkan/private:versions",
    ],
)

bzl_library(
    name = "extensions",
    srcs = ["extensions.bzl"],
    visibility = ["//visibility:public"],
    deps = [":defs"],
)

bzl_library(
    name = "providers",
    srcs = ["providers.bzl"],
    visibility = ["//visibility:public"],
)

bzl_library(
    name = "toolchains",
    srcs = ["toolchains.bzl"],
    visibility = ["//visibility:public"],
)
