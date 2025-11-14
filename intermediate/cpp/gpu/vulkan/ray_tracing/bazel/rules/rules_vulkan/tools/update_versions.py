#!/usr/bin/env python3

# Script to retrieve available SDK versions and file checksums
# based on https://vulkan.lunarg.com/content/view/latest-sdk-version-api

import json
import urllib.request
import urllib.error
import pprint

VERSIONS_URL = "https://vulkan.lunarg.com/sdk/versions.json"
PLATFORMS = ["linux", "mac", "windows", "warm"]

# Fetch list of available Vulkan SDK versions
with urllib.request.urlopen(VERSIONS_URL) as response:
    versions = json.loads(response.read().decode())

print("Available versions:")
for version in versions:
    print(version)

# Find latest version.
version_tuple = lambda v: tuple(map(int, v.split(".")))
latest_version = max(versions, key=version_tuple)
print(f"Latest version: {latest_version}")

PLATFORMS = [
    "linux",
    "mac",
    "windows",
    "warm",
]

# Retrieve checksums.
# See "Get the SHA hash of the SDK file" in https://vulkan.lunarg.com/content/view/latest-sdk-version-api
CHECKSUM_URL = "https://sdk.lunarg.com/sdk/sha/{version}/{platform}/{file}.json"
DOWNLOAD_URL = "https://sdk.lunarg.com/sdk/download/{version}/{platform}/{file}"

# Make package name for download.
# This takes into account name changes that happened in the past.
def make_sdk_name(plat, ver):
    match plat:
        case "linux":
            return "vulkan_sdk.tar.xz"
        case "mac":
            if version_tuple(ver) <= (1, 3, 290, 0):
                return f"vulkansdk-macos-{ver}.dmg"
            else:
                return "vulkan_sdk.zip"
        case "windows":
            if version_tuple(ver) <= (1, 4, 309, 0):
                return f"VulkanSDK-{ver}-Installer.exe"
            else:
                return f"vulkansdk-windows-X64-{ver}.exe"
        case "warm":
            if version_tuple(ver) <= (1, 4, 309, 0):
                return f"InstallVulkanARM64-{ver}.exe"
            else:
                return f"vulkansdk-windows-ARM64-{ver}.exe"
        case _:
            raise ValueError(f"Unknown platform: {plat}")

def make_rt_name(plat, ver):
    template = "VulkanRT-X64-{}-Components.zip" if plat == "windows" else "VulkanRT-ARM64-{}-Components.zip"
    if version_tuple(ver) <= (1, 4, 309, 0) and plat == "windows":
        template = "VulkanRT-{}-Components.zip"
    elif version_tuple(ver) <= (1, 3, 296, 0) and plat == "warm":
        template = "VulkanRT-{}-Components.zip"

    return template.format(ver)

def query(ver, plat, file):
    print(f"Fetching checksum for {ver} on {plat} for file {file}...")

    url = CHECKSUM_URL.format(version=ver, platform=plat, file=file)
    print(f"  URL: {url}")

    # Query checksum URL.
    try:
        with urllib.request.urlopen(url) as resp:
            data = json.loads(resp.read().decode())
    except urllib.error.HTTPError as e:
        if e.code == 404:
            return None
        raise
    print(f"  Result: {data}")

    if isinstance(data, dict) and data.get("ok") is False and data.get("title") == "Not Found":
        return None

    # Extract URL and checksum
    url = DOWNLOAD_URL.format(version=ver, platform=plat, file=data["file"])
    sha = data["sha"]

    return url, sha

output = {}
for ver in versions:
    for plat in PLATFORMS:
        result = query(ver, plat, make_sdk_name(plat, ver))

        # Skip if no such package
        if not result:
            continue

        url, sha = result

        output.setdefault(ver, {})[plat] = {
            "url": url,
            "sha": sha,
        }

        # Check runtime packages on Windows.
        if plat == "windows" or plat == "warm":
            result = query(ver, plat, make_rt_name(plat, ver))
            if result:
                url, sha = result
                output[ver][plat].update({
                    "runtime_url": url,
                    "runtime_sha": sha,
                })


# Sort top-level version keys descending, keep nested dicts as-is
ordered_output = {k: output[k] for k in sorted(output, reverse=True)}

def format_dict(data, indent_level=0):
    """Format a dictionary in Starlark format."""
    if not data:
        return "{}"

    indent = "    " * indent_level
    next_indent = "    " * (indent_level + 1)

    lines = ["{"]

    for key, value in data.items():
        if isinstance(value, dict):
            formatted_value = format_dict(value, indent_level + 1)
            lines.append(f'{next_indent}"{key}": {formatted_value},')
        else:
            lines.append(f'{next_indent}"{key}": "{value}",')
    
    lines.append(f"{indent}}}")
    return "\n".join(lines)

with open("vulkan/private/versions.bzl", "w") as f:
    f.write('"""List of SDK packages currently available for download on LunarG.\n"""\n\n')
    f.write("# GENERATED FILE. Do not edit.\n")
    f.write("# Use ./tools/update_versions.py to update the list of available SDK versions\n\n")
    f.write(f'LATEST_VERSION = "{latest_version}"\n\n')
    f.write("VERSIONS = ")
    f.write(format_dict(ordered_output))
    f.write("\n")

