load("@aspect_bazel_lib//lib:write_source_files.bzl", "write_source_files")
load("@stardoc//stardoc:stardoc.bzl", "stardoc")

stardoc(
    name = "docs",
    out = "index_generated.md",
    input = "//vulkan:defs.bzl",
    symbol_names = [
        # Rules
        "glsl_shader",
        "hlsl_shader",
        "slang_shader",
        "spirv_cross",
        "download_sdk",
        "shader_group",

        # Toolchains
        "vulkan_toolchain",

        # Providers
        "ShaderInfo",
        "ShaderGroupInfo",
    ],
    deps = [
        "//vulkan:defs",
        "//vulkan:providers",
        "//vulkan:toolchains",
    ],
)

write_source_files(
    name = "update",
    files = {
        "index.md": ":docs",
    },
)
