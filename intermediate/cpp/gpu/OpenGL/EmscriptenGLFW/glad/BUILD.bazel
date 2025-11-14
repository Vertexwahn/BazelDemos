load("@rules_python//python:defs.bzl", "py_binary")
load("@my_deps//:requirements.bzl", "requirement")

py_binary(
    name = "run_glad",
    srcs = ["run_glad.py"],
    deps = [
        requirement("glad"),
        requirement("Jinja2"),
    ],
)

RUN_GLAD_CMD = \
    "$(location :run_glad) " + \
    "--profile=core " + \
    "--generator=c " + \
    "--reproducible " + \
    "--local-files " + \
    "--out-path=$(@D)/$(GENDIR)"

genrule(
    name = "gen_glad_hdrs",
    outs = [
        "glad.h",
        "khrplatform.h",
    ],
    cmd = RUN_GLAD_CMD + " ; " +
          "cp $(@D)/$(GENDIR)/glad.h $(@D)/glad.h ; " +
          "cp $(@D)/$(GENDIR)/khrplatform.h $(@D)/khrplatform.h",
    tools = [":run_glad"],
)

genrule(
    name = "gen_glad_srcs",
    outs = [
        "glad.c",
    ],
    cmd = RUN_GLAD_CMD + " ; " +
          "cp $(@D)/$(GENDIR)/glad.c $(@D)/glad.c",
    tools = [":run_glad"],
)

cc_library(
    name = "glad",
    srcs = [":gen_glad_srcs"],
    hdrs = [":gen_glad_hdrs"],
    visibility = ["//visibility:public"],
)
