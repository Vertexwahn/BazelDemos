"""
Module extension to manage Vulkan SDKs.
"""

load("//vulkan:defs.bzl", "download_sdk")

def _vulkan_sdk_impl(ctx):
    for mod in ctx.modules:
        for tag in mod.tags.toolchain:
            name = tag.name
            download_sdk(
                name = name if name else "vulkan_sdk_{}".format(tag.version),
                version = tag.version,
                urls = tag.urls,
            )

    return ctx.extension_metadata(
        reproducible = True,
    )

_toolchain_tag = tag_class(
    attrs = {
        "version": attr.string(
            mandatory = True,
            doc = "SDK version to install",
        ),
        "name": attr.string(
            doc = """
            Optional repository name alias.

            If not specified, will use "vulkan_sdk_{version}".
            """,
        ),
        "urls": attr.string_dict(
            doc = """
            A dictionary of custom URLs and hashes for the Vulkan SDK.

            This allows using a custom mirror for Vulkan SDKs instead of LunarG.

            The structure of the dictionary is similar to the one in `versions.bzl`.

            A separate download URL and sha checksum is required for each platform.
            LunarG currently uses the following platforms:
            - `windows` - for Windows 64-bit
            - `warm` - Windows ARM64.
            - `mac` - Mac OS
            - `linux` - Linux

            Each entry required to provide `url` and `sha` fields.
            Additionally, on Windows platforms, `runtime_url` and `runtime_sha` can be used to provide URLs
            for the runtime package.

            Example:
            ```bazel
            custom_urls = {
                "linux": {
                    "url": "https://sdk.lunarg.com/sdk/download/1.4.313.0/linux/vulkansdk-linux-x86_64-1.4.313.0.tar.xz",
                    "sha": "4e957b66ade85eeaee95932aa7e3b45aea64db373c58a5eaefc8228cc71445c2",
                },
                "mac": {
                    "url": "https://sdk.lunarg.com/sdk/download/1.4.313.0/mac/vulkansdk-macos-1.4.313.0.zip",
                    "sha": "782a966ef4d5d68acaa933ff45215df2e34f286df8f6077270202f218110dc20",
                },
                "windows": {
                    "url": "https://sdk.lunarg.com/sdk/download/1.4.313.0/windows/vulkansdk-windows-X64-1.4.313.0.exe",
                    "sha": "b643ca8ab4aea5c47b9c4e021a0b33b3a13871bf1d8131e162a9e48c257c4694",
                    "runtime_url": "https://sdk.lunarg.com/sdk/download/1.4.313.0/windows/VulkanRT-X64-1.4.313.0-Components.zip",
                    "runtime_sha": "e8d37913185142270a2bc1b3e1f8f498a4edf47405fddda666f2f38b30ca944b",
                },
                "warm": {
                    "url": "https://sdk.lunarg.com/sdk/download/1.4.313.0/warm/vulkansdk-windows-ARM64-1.4.313.0.exe",
                    "sha": "b19a8683df982d302fec07c110962153f02a2e5cf1e5118ff72d8532aa5fc567",
                    "runtime_url": "https://sdk.lunarg.com/sdk/download/1.4.313.0/warm/VulkanRT-ARM64-1.4.313.0-Components.zip",
                    "runtime_sha": "6335a8d6b7ab85861025c2546f5f52384ff18a6d9346d350c2a0bf3b7524829a",
                },
            }
            ```
            """,
        ),
    },
)

vulkan_sdk = module_extension(
    implementation = _vulkan_sdk_impl,
    doc = """
    Module extension to manage Vulkan SDKs.
    """,
    tag_classes = {
        "toolchain": _toolchain_tag,
    },
    os_dependent = True,
    arch_dependent = True,
)
