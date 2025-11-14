"""
An example how to aggregate information from a group of shaders.

This demonstrates how to use ShaderInfo to build shader databases by:
- Collecting shader metadata (entry points, stages, compilation targets)
- Organizing additional compiler outputs (assembly, reflection, dependencies)
- Creating summary statistics about the shader collection
- Generating a structured JSON database for runtime use
"""

load("@rules_vulkan//vulkan:providers.bzl", "ShaderGroupInfo", "ShaderInfo")

def _shader_metadata_impl(ctx):
    shader_database = []
    reflection_files = []
    assembly_files = []
    dependency_files = []

    for dep in ctx.attr.deps:
        shader_infos = []
        if ShaderInfo in dep:
            shader_infos = [dep[ShaderInfo]]
        elif ShaderGroupInfo in dep:
            shader_infos = dep[ShaderGroupInfo].list

        # Process each shader and extract metadata
        for info in shader_infos:
            shader_entry = {
                "entry_point": info.entry,
                "stage": info.stage,
                "target": info.target,
                "defines": info.defines,
                "compiled_shader": [f.path for f in dep[DefaultInfo].files.to_list()],
            }

            # Add optional output files if they exist
            optional_outputs = {}
            if hasattr(info, "assembly") and info.assembly:
                optional_outputs["assembly"] = info.assembly.path
                assembly_files.append(info.assembly)
            if hasattr(info, "reflection") and info.reflection:
                optional_outputs["reflection"] = info.reflection.path
                reflection_files.append(info.reflection)
            if hasattr(info, "hash") and info.hash:
                optional_outputs["hash"] = info.hash.path
            if hasattr(info, "depfile") and info.depfile:
                optional_outputs["depfile"] = info.depfile.path
                dependency_files.append(info.depfile)

            if optional_outputs:
                shader_entry["additional_outputs"] = optional_outputs

            shader_database.append(shader_entry)

    # Create summary statistics
    stages_set = {}
    targets_set = {}
    for shader in shader_database:
        if shader["stage"]:
            stages_set[shader["stage"]] = True
        if shader["target"]:
            targets_set[shader["target"]] = True

    summary = {
        "total_shaders": len(shader_database),
        "stages": sorted(stages_set.keys()),
        "targets": sorted(targets_set.keys()),
        "reflection_files_count": len(reflection_files),
        "assembly_files_count": len(assembly_files),
        "dependency_files_count": len(dependency_files),
    }

    metadata = {
        "summary": summary,
        "shaders": shader_database,
        "reflection_files": [f.path for f in reflection_files],
        "assembly_files": [f.path for f in assembly_files],
        "dependency_files": [f.path for f in dependency_files],
    }

    out = ctx.actions.declare_file(ctx.attr.out)
    ctx.actions.write(
        output = out,
        content = json.encode_indent(metadata, indent = "  "),
    )

    return [DefaultInfo(files = depset([out]))]

gather_metadata = rule(
    implementation = _shader_metadata_impl,
    attrs = {
        "out": attr.string(
            mandatory = True,
            doc = "JSON file name to write metadata output",
        ),
        "deps": attr.label_list(
            doc = """
	    List of shader targets to gather metadata from.
	    """,
            providers = [[ShaderInfo], [ShaderGroupInfo]],
        ),
    },
)
