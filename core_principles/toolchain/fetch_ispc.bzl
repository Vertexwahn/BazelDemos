"""ISPC compiler fetch"""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def fetch_ispc():
    """function which fetches the remote prebuild ISPC compiler
    """

    # For download link see https://ispc.github.io/downloads.html

    http_archive(
        name = "ispc_osx_arm64",
        urls = [
            "https://github.com/ispc/ispc/releases/download/v1.19.0/ispc-v1.19.0-macOS.arm.tar.gz",
        ],
        sha256 = "47b9fba87affde0842752dc6485bd544c13030872cac1190fb4354fe30580b8c",
        strip_prefix = "ispc-v1.19.0-macOS.arm",
        build_file = "@rules_ispc//:ispc_osx_arm64.BUILD",
    )
