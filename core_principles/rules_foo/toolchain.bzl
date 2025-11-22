FOO_TOOLCHAIN = "@rules_foo//:toolchain_type"

def _foo_toolchain(ctx):
    return [
        platform_common.ToolchainInfo(
            compiler = ctx.attr.compiler,
            flags = ctx.attr.flags,
        ),
    ]

foo_toolchain = rule(
    _foo_toolchain,
    attrs = {
        "compiler": attr.label(
            executable = True,
            default = "//:demo_compiler",
            cfg = "host",
        ),
        "flags": attr.string_list(),
    },
)
