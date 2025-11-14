"""
A rule to compile GLSL shaders.
"""

load("@bazel_skylib//rules:common_settings.bzl", "BuildSettingInfo")
load("//vulkan:providers.bzl", "ShaderInfo")

def _hlsl_shader_impl(ctx):
    sdk = ctx.toolchains["//vulkan:toolchain_type"].info

    compiled_file = ctx.actions.declare_file(ctx.label.name + ".out")
    all_files = [compiled_file]

    args = ctx.actions.args()

    args.add_all([
        "-o",
        compiled_file,
        "-fshader-stage={}".format(ctx.attr.stage),
    ])

    for define in ctx.attr.defines:
        args.add(define, format = "-D%s")

    for path in ctx.attr.includes:
        args.add("-I", path)

    if ctx.attr.std:
        args.add(ctx.attr.std, format = "-std=%s")

    if ctx.attr.target_env:
        args.add(ctx.attr.target_env, format = "--target-env=%s")

    if ctx.attr.target_spv:
        args.add(ctx.attr.target_spv, format = "--target-spv=%s")

    # Append user-defined extra arguments
    args.add_all(ctx.attr.opts)

    # Append build settings options.
    extra_opts = ctx.attr._extra_opts[BuildSettingInfo].value
    args.add_all(extra_opts, uniquify = True)

    # Specify shader inputs.
    src = ctx.file.src
    args.add(src.path)

    ctx.actions.run(
        inputs = [src] + ctx.files.hdrs,
        outputs = all_files,
        arguments = [args],
        executable = sdk.glslc,
        progress_message = "Compiling GLSL shader %s" % src.path,
        mnemonic = "GlslCompile",
    )

    return [
        DefaultInfo(
            files = depset([compiled_file]),
        ),
        OutputGroupInfo(
            all_files = depset(all_files),
        ),
        ShaderInfo(
            binary = compiled_file,
            entry = "main",
            assembly = None,
            reflection = None,
            hash = None,
            depfile = None,
            stage = ctx.attr.stage,
            defines = ctx.attr.defines,
            target = ctx.attr.target_spv,
        ),
    ]

glsl_shader = rule(
    implementation = _hlsl_shader_impl,
    doc = """
    Rule to compile GLSL shader.
    """,
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
            doc = "Input GLSL shader source to compile",
        ),
        "stage": attr.string(
            mandatory = True,
            doc = "Shader stage (vertex, vert, fragment, frag, etc)",
        ),
        "includes": attr.string_list(
            doc = "Add directory to include search path to CLI",
        ),
        "hdrs": attr.label_list(
            allow_files = True,
            doc = "List of header files dependencies to be included in the shader compilation",
        ),
        "defines": attr.string_list(
            doc = "List of macro defines",
        ),
        "std": attr.string(
            doc = """
            Version and profile for GLSL input files.

            Possible values are concatenations of version and profile, e.g. `310es`, `450core`, etc.
            """,
        ),
        "target_env": attr.string(
            doc = """
            Set the target client environment, and the semantics of warnings and errors.

            An optional suffix can specify the client version.
            """,
        ),
        "target_spv": attr.string(
            doc = """
            Set the SPIR-V version to be used for the generated SPIR-V module.

            The default is the highest version of SPIR-V required to be supported for the target environment.
            For example, default for `vulkan1.0` is `spv1.0`.
            """,
        ),
        "opts": attr.string_list(
            doc = "Additional arguments to pass to the compiler",
        ),
        "_extra_opts": attr.label(
            default = "//vulkan/settings:glslc_opts",
            doc = "Add extra options provided via Bazel's build settings.",
        ),
    },
    toolchains = ["//vulkan:toolchain_type"],
    provides = [ShaderInfo],
)
