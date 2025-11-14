"""
spirv-cross wrapper
"""

load("//vulkan:providers.bzl", "ShaderInfo")

def _spirv_cross_impl(ctx):
    sdk = ctx.toolchains["//vulkan:toolchain_type"].info

    input_file = ctx.file.src

    output_name = ctx.attr.out if ctx.attr.out else ctx.label.name + ".out"
    output_file = ctx.actions.declare_file(output_name)

    args = ctx.actions.args()
    args.add_all(["--output", output_file])

    if ctx.attr.backend:
        args.add(ctx.attr.backend, format = "--%s")

    shader_info = ctx.attr.src[ShaderInfo]

    stage = None
    if ctx.attr.stage:
        stage = ctx.attr.stage
    elif shader_info:
        stage = shader_info.stage

    if stage:
        args.add("--stage", stage)

    entry = None
    if ctx.attr.entry:
        entry = ctx.attr.entry
    elif shader_info:
        entry = shader_info.entry

    if entry:
        args.add("--entry", entry)

    args.add_all(ctx.attr.opts)
    args.add(input_file)

    ctx.actions.run(
        inputs = [input_file],
        outputs = [output_file],
        arguments = [args],
        executable = sdk.spirv_cross,
        progress_message = "Running spirv-cross on %s" % input_file.basename,
    )

    return [
        DefaultInfo(files = depset([output_file])),
        ShaderInfo(
            binary = output_file,
            entry = entry,
            assembly = None,
            reflection = None,
            hash = None,
            depfile = None,
            stage = stage,
            defines = shader_info.defines if shader_info else [],
            target = ctx.attr.backend,
        ),
    ]

spirv_cross = rule(
    implementation = _spirv_cross_impl,
    toolchains = ["//vulkan:toolchain_type"],
    doc = """
    spirv-cross build target.

    This rule allows invoking spirv-cross binary bundled with Vulkan SDK.

    Additionally, it integrates with compiler rules (hlsl_shader, glsl_shader, etc) and can inherit entry point
    and stages from `CompilerInfo`.
    """,
    attrs = {
        "src": attr.label(
            mandatory = True,
            allow_single_file = True,
            doc = "Input file",
        ),
        "out": attr.string(
            doc = "Output artifact name. If not specified, will default to <name>.out",
        ),
        "backend": attr.string(
            doc = "Select backend for spirv-cross",
            values = ["vulkan-semantics", "msl", "hlsl", "reflect", "cpp"],
        ),
        "stage": attr.string(
            doc = "Forces use of a certain shader stage",
        ),
        "entry": attr.string(
            doc = "Use a specific entry point",
        ),
        "opts": attr.string_list(
            doc = "Additional arguments to pass to the spirv-cross binary",
        ),
    },
)
