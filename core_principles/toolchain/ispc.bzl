"""A rule that compiles ISPC programs and make them available as a C++ library.
"""

def _ispc_cc_library_impl(ctx):
    info = ctx.toolchains["@rules_ispc//tools:toolchain_type"].ispc_info
    ispc_path = info.ispc_path

    generated_header_filename = ctx.attr.generated_header_filename

    srcs = ctx.files.srcs
    inputs = depset(srcs)  # see https://bazel.build/extending/rules

    object = ctx.actions.declare_file(ctx.attr.name + ".o")

    args = ctx.actions.args()
    args.add("--target-os=%s" % "macos")
    args.add("--arch=aarch64")
    args.add("--target=neon")
    args.add("--addressing=64")
    args.add("--pic")
    args.add(ctx.file.ispc_main_source_file.short_path)
    args.add("--header-outfile=%s" % "defines/square.h")  #generated_header_filename.short_path)
    args.add("-o", object)

    exec_requirements = {}
    for elem in ctx.attr.tags:
        exec_requirements[elem] = "1"

    ctx.actions.run(
        inputs = inputs,
        outputs = [object, ctx.outputs.generated_header_filename],
        arguments = [args],
        executable = ispc_path,
        execution_requirements = exec_requirements,
    )

    return [
        DefaultInfo(files = depset(direct = [object])),
    ]

ispc_library2 = rule(
    implementation = _ispc_cc_library_impl,
    doc = """Compiles a ISPC program and makes it available as a C++ library

This rule uses a precompiled version of ISPC v1.19.0 for compilation.""",
    attrs = {
        "generated_header_filename": attr.output(
            doc = """
            Name of the generated header file.
            """,
        ),
        "ispc_main_source_file": attr.label(
            allow_single_file = [".ispc"],
            doc = """
            File to compile.
            """,
        ),
        "srcs": attr.label_list(
            allow_files = [".ispc", ".isph"],
            doc = """
            The list of ISPC source files that are compiled to create the library.
            Only `.ispc` and `.isph` files are permitted.
            """,
        ),
        "defines": attr.string_list(
            doc = """
            List of defines handed over to the ISPC compiler.
            """,
        ),
    },
    toolchains = ["@rules_ispc//tools:toolchain_type"],
)

def ispc_cc_library2(name, generated_header_filename, ispc_main_source_file, srcs, defines = [], **kwargs):
    ispc_library2(
        name = "%s_ispc_gen" % name,
        generated_header_filename = generated_header_filename,
        ispc_main_source_file = ispc_main_source_file,
        srcs = srcs,
        defines = defines,
        tags = ["local"],
        **kwargs
    )
