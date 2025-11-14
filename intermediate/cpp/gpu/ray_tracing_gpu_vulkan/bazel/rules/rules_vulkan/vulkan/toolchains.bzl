"""
Vulkan SDK toolchains
"""

VulkanInfo = provider(
    doc = """Information about Vulkan SDK""",
    fields = ["dxc", "slangc", "glslc", "spirv_cross", "env"],
)

def _vulkan_toolchain_impl(ctx):
    toolchain_info = platform_common.ToolchainInfo(
        info = VulkanInfo(
            dxc = ctx.executable.dxc,
            slangc = ctx.executable.slangc,
            glslc = ctx.executable.glslc,
            spirv_cross = ctx.executable.spirv_cross,
            env = ctx.attr.env,
        ),
    )

    return [toolchain_info]

vulkan_toolchain = rule(
    implementation = _vulkan_toolchain_impl,
    attrs = {
        "dxc": attr.label(
            doc = "Path to dxc",
            executable = True,
            mandatory = True,
            cfg = "exec",
        ),
        "slangc": attr.label(
            doc = "Path to slangc",
            executable = True,
            mandatory = True,
            cfg = "exec",
        ),
        "glslc": attr.label(
            doc = "Path to glslc",
            executable = True,
            mandatory = True,
            cfg = "exec",
        ),
        "spirv_cross": attr.label(
            doc = "Path to spirv_cross",
            executable = True,
            mandatory = True,
            cfg = "exec",
        ),
        "env": attr.string_dict(
            doc = """
            Environment to be passed to executables.
            """,
        ),
    },
)
