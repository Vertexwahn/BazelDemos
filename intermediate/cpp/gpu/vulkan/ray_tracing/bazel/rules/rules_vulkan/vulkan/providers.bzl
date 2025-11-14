"""
Vulkan providers.
"""

ShaderInfo = provider(
    doc = """
    Shader metadata returned by the shader targets during compilation.

    This is useful for building all kind of shader databases.
    """,
    fields = {
        "binary": "Binary output file containing the compiled shader bytecode",
        "assembly": "Assembly output file (if generated, HLSL-specific)",
        "reflection": "Reflection output file (if generated)",
        "hash": "Hash output file (if generated, HLSL-specific)",
        "depfile": "Dependency output file (if generated, Slang-specific)",
        "entry": "Shader entry point function name",
        "stage": "Shader stage",
        "defines": "List of shader defines used during compilation",
        "target": "Compilation target (note: this depends on compiler used)",
    },
)

ShaderGroupInfo = provider(
    doc = """
    A collection of shader infos.
    """,
    fields = {
        "list": "List of ShaderInfo structures",
    },
)
