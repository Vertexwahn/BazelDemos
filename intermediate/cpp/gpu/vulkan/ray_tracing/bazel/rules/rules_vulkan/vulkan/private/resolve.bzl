"""
Module to resolve download URL and checksum from the provided SDK version.
"""

load("@aspect_bazel_lib//lib:repo_utils.bzl", "repo_utils")
load(":versions.bzl", "VERSIONS")

def normalize_os(ctx):
    """
    Convert repository context to LunarG platform name using repo_utils.

    Args:
        ctx: Repository context.
    Returns:
        Platform name
    """
    platform = repo_utils.platform(ctx)

    # Map repo_utils platform names (os_arch) to LunarG platform names
    platform_mapping = {
        "darwin_amd64": "mac",
        "darwin_arm64": "mac",
        "linux_amd64": "linux",
        "linux_arm64": "linux",
        "linux_s390x": "linux",
        "linux_ppc64le": "linux",
        "windows_amd64": "windows",
        "windows_arm64": "warm",
    }

    vulkan_platform = platform_mapping.get(platform)
    if not vulkan_platform:
        fail("Unsupported platform: {}".format(platform))

    return vulkan_platform

def normalize_version(version):
    """
    Normalize SDK version string to ensure it has 4 components.

    Args:
        version: Version string in the format 'X.Y.Z' or 'X.Y.Z.W'
    Returns:
        Version as 'X.Y.Z.0' if only three components, else returns as-is."""
    parts = version.split(".")
    if len(parts) == 3:
        return version + ".0"

    return version

def find_exact(version):
    """
    Find exact record in the VERSIONS table from the provided version and OS/arch retrieved from the `ctx`

    Args:
        version: exact version string to fetch info.

    Returns:
        A dictionary with URLs for each supported platform.
	Note: LunarG may release patch releases for certain platforms, so some platform entries might be missing.
    """
    platforms = VERSIONS.get(version, None)
    if not platforms:
        fail("Unsupported Vulkan SDK version: {}".format(version))

    return platforms
