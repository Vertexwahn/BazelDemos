"""
Common Vulkan rules.
"""

load("@bazel_skylib//lib:paths.bzl", "paths")
load("@rules_pkg//pkg:providers.bzl", "PackageFilesInfo")
load("//vulkan:providers.bzl", "ShaderGroupInfo", "ShaderInfo")

def _shader_group_impl(ctx):
    all_files = []
    all_infos = []
    src_map = {}

    prefix = ctx.attr.pkg_prefix or ""

    for dep in ctx.attr.deps:
        if ShaderInfo in dep:
            all_infos.append(dep[ShaderInfo])
        elif ShaderGroupInfo in dep:
            all_infos.extend(dep[ShaderGroupInfo].list)

        # Recursively merge dest_src_map if dep already provides PackageFilesInfo
        if PackageFilesInfo in dep:
            for k, v in dep[PackageFilesInfo].dest_src_map.items():
                src_map[paths.join(prefix, k)] = v
        else:
            files = []
            if hasattr(dep[OutputGroupInfo], "all_files"):
                files = dep[OutputGroupInfo].all_files.to_list()
            else:
                files = dep[DefaultInfo].files.to_list()

            for f in files:
                src_map[paths.join(prefix, f.basename)] = f

    all_files.extend(src_map.values())

    return [
        DefaultInfo(files = depset(all_files)),
        ShaderGroupInfo(list = all_infos),
        PackageFilesInfo(dest_src_map = src_map),
    ]

shader_group = rule(
    implementation = _shader_group_impl,
    doc = """
    `shader_group` is a rule to group multiple shaders together.

    Similar to `filegroup`, but forwards providers to enable building shader libraries and databases. The motivation
    for this rule is described in this [issue](https://github.com/bazelbuild/bazel/issues/8904).

    The rule expects dependencies from shader compiler targets such as `hlsl_shader`, `glsl_shader`, `slang_shader`,
    and `spirv_cross`. Shader groups can also depend on other shader groups, creating hierarchical structures. Each
    `shader_group` accumulates ShaderInfo structures from all its dependencies and returns a ShaderGroupInfo provider
    containing the collected shader information.

    Common use cases include:
    - Grouping related shaders together (e.g. vertex + fragment shader pair).
    - Building large shader libraries or databases. See the `e2e/smoke` example for implementation details.

    **Example:**

    ```starlark
    load("@rules_vulkan//vulkan:defs.bzl", "hlsl_shader", "glsl_shader", "shader_group")

    hlsl_shader(
        name = "vertex_shader",
        src = "vertex.hlsl",
        entry = "VSMain",
        target = "vs_6_0",
    )

    glsl_shader(
        name = "fragment_shader",
        src = "fragment.glsl",
        stage = "frag",
    )

    shader_group(
        name = "graphics_shaders",
        deps = [
            ":vertex_shader",
            ":fragment_shader",
        ],
        pkg_prefix = "shaders/",
    )
    ```

    **Rules_pkg Integration:**

    The `shader_group` rule integrates seamlessly with `rules_pkg` for creating shader archives. When `pkg_prefix` is
    specified, the rule provides `PackageFilesInfo` that can be consumed directly by `pkg_zip` and other packaging
    rules.

    The shader group acts as a replacement for `pkg_files`, automatically bundling all shader outputs (compiled
    binaries, reflection data, assembly files, etc.) into the specified archive subdirectory. This eliminates the
    need to manually specify each shader output file when creating packages.

    ```starlark
    load("@rules_pkg//:pkg.bzl", "pkg_zip")

    pkg_zip(
        name = "shader_database",
        srcs = [":graphics_shaders"],  # All shaders will be placed in shaders/ subdirectory
        out = "shaders.zip",
    )
    ```
    """,
    attrs = {
        "deps": attr.label_list(
            doc = """
	    List of shader targets to group together.

	    Accepts individual shader targets (HLSL, GLSL, Slang, or spirv_cross) and other `shader_group` targets for
	    hierarchical grouping.
	    """,
            allow_files = True,
            providers = [[ShaderInfo], [ShaderGroupInfo]],
        ),
        "pkg_prefix": attr.string(
            doc = "If using with `rules_pkg`, sub-directory in the destination archive",
        ),
    },
    provides = [ShaderGroupInfo, PackageFilesInfo],
)
