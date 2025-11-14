"""
Vulkan SDK downloader.
"""

load("@aspect_bazel_lib//lib:repo_utils.bzl", "repo_utils")
load(":resolve.bzl", "find_exact", "normalize_os", "normalize_version")

def _install_linux(ctx, urls, version, attrs):
    ctx.report_progress("Downloading and unpacking tarball...")
    ctx.download_and_extract(
        urls["url"],
        sha256 = urls["sha"],
        output = "unpack",
        stripPrefix = version,
    )

    ctx.symlink("unpack/x86_64/", "sdk")

    attrs.update({
        "{os}": "linux",
        "{sdk_root}": str(ctx.path("sdk")),
        "{lib_vulkan}": "sdk/lib/libvulkan*.so*",
    })

def _install_macos(ctx, urls, version, attrs):
    ctx.report_progress("Downloading installer...")
    ctx.download_and_extract(
        urls["url"],
        sha256 = urls["sha"],
        output = "installer",
    )

    # Install Vulkan components from terminal
    # See https://vulkan.lunarg.com/doc/view/1.3.283.0/mac/getting_started.html
    #
    # The installer downloads components from internet, which Bazel can't really track.
    # And hence this gets invoked every time any dependent targets are built.
    #
    # To mitigate this, this will check if "sdk" symlink already exists, and if so, the
    # download will be skipped.
    #
    # This is not ideal and might need to be revisited in future, but it works.
    if not ctx.path("sdk").exists:
        ctx.report_progress("Running installer...")
        ctx.execute(
            [
                "./installer/vulkansdk-macOS-{0}.app/Contents/MacOS/vulkansdk-macOS-{0}".format(version),
                "--root",
                ctx.path("unpack"),  # Warning: The installation path cannot be relative, please specify an absolute path.
                "--accept-licenses",
                "--default-answer",
                "--confirm-command",
                "install",
                # Optional components to install.
                # TODO: Make optional components configurable.
                "com.lunarg.vulkan.vma",
            ],
            quiet = False,
        )

        ctx.symlink("unpack/macOS/", "sdk")

    attrs.update({
        "{os}": "macos",
        "{sdk_root}": str(ctx.path("sdk")),
        "{lib_vulkan}": "sdk/lib/libvulkan*.dylib",
    })

def _install_windows(ctx, urls, version, attrs):
    ctx.report_progress("Downloading installer...")
    ctx.download(
        urls["url"],
        sha256 = urls["sha"],
        output = "installer.exe",
    )

    skip_rt = ctx.attr.windows_skip_runtime

    if not skip_rt:
        is_arm = ctx.os.arch.startswith("arm")

        ctx.report_progress("Downloading runtime...")
        ctx.download_and_extract(
            urls["runtime_url"],
            sha256 = urls["runtime_sha"],
            strip_prefix = "VulkanRT-{}-{}-Components\\{}".format(
                "ARM64" if is_arm else "X64",
                version,
                "" if is_arm else "x64",  # on x64 there is an additional x64 subdirectory
            ),
        )

        attrs.update({"{vulkan_deps}": "\":vulkan_dll\""})

    # See https://vulkan.lunarg.com/doc/sdk/latest/windows/getting_started.html
    ctx.report_progress("Installing components...")

    ctx.execute([
        "cmd.exe",
        "/c",
        "installer.exe",
        "--root",
        ctx.path("sdk"),
        "--verbose",
        "--accept-licenses",
        "--default-answer",
        "--confirm-command install",
        # For completely unattended installation and modifications,
        # the command prompt must be run as administrator.
        # There is an option to only copy the SDK files and not perform any operations to the registry
        # such as setting up new layers, creating shortcuts, and adjustments to the system path.
        # For the copy only option, append copy_only=1 to the end of the command line installer executable.
        "copy_only=1",
    ], quiet = False)

    attrs.update({
        "{os}": "windows",
        "{sdk_root}": str(ctx.path("sdk")),
        "{include_path}": "sdk/Include",
        "{lib_vulkan}": "sdk/Lib/vulkan*.lib",
    })

def _download_impl(ctx):
    version = ctx.attr.version
    if not version:
        fail("Vulkan SDK version must be specified")
    version = normalize_version(version)

    # Fetch the list of download URLs for the provided SDK version
    urls = ctx.attr.urls
    if not urls:
        # Fetch URLs for known SDK versions
        urls = find_exact(version)

    # Fetch URLs for the current platform
    platform = normalize_os(ctx)
    urls = urls.get(platform, None)
    if not urls:
        fail("Download URLs not found for platform {} and SDK {}", platform, version)

    attrs = {
        "{os}": "",
        "{include_path}": "sdk/include",
        "{lib_vulkan}": "sdk/lib/libvulkan*.so*",
        "{vulkan_deps}": "",  # Windows only
    }

    if repo_utils.is_linux(ctx):
        _install_linux(ctx, urls, version, attrs)
    elif repo_utils.is_darwin(ctx):
        _install_macos(ctx, urls, version, attrs)
    elif repo_utils.is_windows(ctx):
        _install_windows(ctx, urls, version, attrs)
    else:
        fail("Unsupported OS: {}".format(platform))

    ctx.template("BUILD", ctx.attr.build_file, executable = False, substitutions = attrs)

download_sdk = repository_rule(
    implementation = _download_impl,
    doc = """
    A rule to handle download and unpack of the SDK for each major platform (Windows, Linux, MacOS).

    These rely on command line installation described in "Getting started" docs on LunarG.
    - https://vulkan.lunarg.com/doc/view/1.3.283.0/mac/getting_started.html

    """,
    attrs = {
        "urls": attr.string_dict(
            mandatory = True,
            doc = """
            URLs and SHA256 checksum for each platform.

            Refer to the module extension doc for specifics.
            """,
        ),
        "version": attr.string(
            mandatory = True,
            doc = """
	    Vulkan SDK version to download and install.

	    This expects a version in the format of `1.4.313.0` or `1.4.313`.
	    When 3 components are provided, `.0` will be appended automatically to make it 4 components.
            """,
        ),
        "windows_skip_runtime": attr.bool(
            default = False,
            doc = """
            Do not download and install Vulkan runtime package (e.g. `vulkan-1.dll` dependency) on Windows.

            When `True`, the downloader will put `vulkan-1.dll` into the repository root directory.

            This is useful if there is a system-wide Vulkan runtime already installed, otherwise this
            might lead to link/runtime issues when building CC targets.
            """,
        ),
        "build_file": attr.label(default = Label("//vulkan/private:template.BUILD")),
    },
)
