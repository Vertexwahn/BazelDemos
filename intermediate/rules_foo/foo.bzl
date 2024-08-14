load("@rules_cc//cc:defs.bzl", "cc_library")

def _foo_library_impl(ctx):
    ctx.actions.write(ctx.outputs.out, content = "#define PI 3.14")

    return [DefaultInfo(
        files = depset([ctx.outputs.out]),
    )]

foo_library = rule(
    implementation = _foo_library_impl,

    attrs = {
        "out": attr.output(
            doc = """
            Name of the generated header file.
            """,
            mandatory = True,
        ),
    },
    toolchains = ["@rules_foo//:toolchain_type"],

    doc = "Generates a header file",
)

def foo_cc_library(name, **kwargs):
    foo_library(
        name = "{0}_foo_library_generated".format(name),
        out = "{0}.h".format(name)
    )

    cc_library(
        name = name,
        hdrs = [name + ".h"],
        **kwargs
    )
