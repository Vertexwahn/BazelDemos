"""
A rule to compile HLSL shaders using DirectXShaderCompiler (dxc).
"""

load("@bazel_skylib//rules:common_settings.bzl", "BuildSettingInfo")
load("//vulkan:providers.bzl", "ShaderInfo")

# Normalize stage names
# See https://github.com/KhronosGroup/SPIRV-Cross/blob/d8e3e2b141b8c8a167b2e3984736a6baacff316c/main.cpp#L1151
def _map_stage(target):
    if target.startswith("vs"):
        return "vert"
    elif target.startswith("ps"):
        return "frag"
    elif target.startswith("cs"):
        return "comp"
    elif target.startswith("gs"):
        return "geom"
    elif target.startswith("hs"):
        return "tesc"
    elif target.startswith("tese"):
        return "domain"
    elif target.startswith("ms"):
        return "mesh"
    elif target.startswith("as"):
        return "task"
    else:
        return "unknown"

def _hlsl_shader_impl(ctx):
    sdk = ctx.toolchains["//vulkan:toolchain_type"].info

    ext = ".spv" if ctx.attr.spirv else ".cso"
    compiled_file = ctx.actions.declare_file(ctx.label.name + ext)
    all_files = [compiled_file]

    args = ctx.actions.args()

    # Target + shader output path (required).
    args.add_all(["-T", ctx.attr.target, "-Fo", compiled_file])

    # Entry point
    if ctx.attr.entry:
        args.add("-E", ctx.attr.entry)

    # Append macro defines
    for define in ctx.attr.defines:
        args.add("-D", define)

    for path in ctx.attr.includes:
        args.add("-I", path)

    # Specify HLSL version
    if ctx.attr.hlsl:
        args.add("-HV", ctx.attr.hlsl)

    # Specify root signature from #define
    if ctx.attr.def_root_sig:
        args.add("-rootsig-define", ctx.attr.def_root_sig)

    # Output assembly code
    asm_file = None
    if ctx.attr.asm:
        asm_file = ctx.actions.declare_file(ctx.attr.asm)
        args.add("-Fc", asm_file)
        all_files.append(asm_file)

    # Output reflection
    reflection_file = None
    if ctx.attr.reflect:
        reflection_file = ctx.actions.declare_file(ctx.attr.reflect)
        args.add("-Fre", reflection_file)
        all_files.append(reflection_file)

    # Output hash.
    hash_file = None
    if ctx.attr.hash:
        hash_file = ctx.actions.declare_file(ctx.attr.hash)
        args.add("-Fsh", hash_file)
        all_files.append(hash_file)

    if ctx.attr.spirv:
        args.add("-spirv")

    # Append user-defined extra arguments
    args.add_all(ctx.attr.opts)

    # Append build settings options.
    extra_opts = ctx.attr._extra_opts[BuildSettingInfo].value
    args.add_all(extra_opts, uniquify = True)

    # Specify input shader source file
    src = ctx.file.src
    args.add(src.path)

    ctx.actions.run(
        inputs = [src] + ctx.files.hdrs,
        outputs = all_files,
        arguments = [args],
        executable = sdk.dxc,
        env = sdk.env,
        progress_message = "Compiling HLSL shader %s" % src.path,
        mnemonic = "HlslCompile",
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
            entry = ctx.attr.entry,
            assembly = asm_file if asm_file else None,
            reflection = reflection_file if reflection_file else None,
            hash = hash_file if hash_file else None,
            depfile = None,
            stage = _map_stage(ctx.attr.target),
            defines = ctx.attr.defines,
            target = ctx.attr.target,
        ),
    ]

hlsl_shader = rule(
    implementation = _hlsl_shader_impl,
    doc = """
    Rule to compile HLSL shaders using DirectXShaderCompiler.

    The target will output <name>.cso or <name>.spv (when targeting spirv) file with bytecode output.
    """,
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            mandatory = True,
            doc = "Input HLSL shader source file",
        ),
        "entry": attr.string(
            default = "main",
            doc = "Entry point name",
        ),
        "target": attr.string(
            mandatory = True,
            doc = "Target profile (e.g., cs_6_0, ps_6_0, etc.)",
        ),
        "defines": attr.string_list(
            doc = "List of macro defines",
        ),
        "includes": attr.string_list(
            doc = "List of directories to be added to the CLI to search for include files",
        ),
        "hdrs": attr.label_list(
            allow_files = True,
            doc = "List of header files dependencies to be included in the shader compilation",
        ),
        "hlsl": attr.string(
            doc = "HLSL version to use (2016, 2017, 2018, 2021)",
        ),
        "def_root_sig": attr.string(
            doc = "Read root signature from a #define (-rootsig-define <value>)",
        ),
        "spirv": attr.bool(
            doc = "Generate SPIR-V code",
        ),
        "opts": attr.string_list(
            doc = "Additional arguments to pass to the DXC compiler",
        ),
        "asm": attr.string(
            doc = "Output assembly code listing file to the specified path (-Fc <file>)",
        ),
        "reflect": attr.string(
            doc = "Output reflection to the specified file (-Fre <file>)",
        ),
        "hash": attr.string(
            doc = "Output shader hash to the specified file (-Fsh <file>)",
        ),
        "_extra_opts": attr.label(
            default = "//vulkan/settings:dxc_opts",
            doc = "Add extra options provided via Bazel's build settings.",
        ),
    },
    toolchains = ["//vulkan:toolchain_type"],
    provides = [ShaderInfo],
)
