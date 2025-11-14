load("@buildifier_prebuilt//:rules.bzl", "buildifier")
load("@gazelle//:def.bzl", "gazelle", "gazelle_binary")

exports_files(["LICENSE"])

buildifier(name = "fmt")

buildifier(
    name = "lint",
    lint_mode = "warn",
    mode = "check",
)

gazelle(
    name = "gazelle",
    gazelle = ":gazelle_bin",
)

gazelle_binary(
    name = "gazelle_bin",
    languages = [
        "@bazel_skylib_gazelle_plugin//bzl",
    ],
)
