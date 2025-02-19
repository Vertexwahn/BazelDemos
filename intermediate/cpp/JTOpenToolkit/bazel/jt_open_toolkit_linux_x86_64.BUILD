load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

cc_import(
    name = "libJt-10.6.0.so",
    shared_library = "lib/linux_64/libJt-10.6.0.so",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libParaSupt-10.6.0.so",
    shared_library = "lib/linux_64/libParaSupt-10.6.0.so",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libpskernel.so",
    shared_library = "lib/linux_64/libpskernel.so",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libpsbodyshop.so",
    shared_library = "lib/linux_64/libpsbodyshop.so",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libpsxttoolkit.so",
    shared_library = "lib/linux_64/libpsxttoolkit.so",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libeaiunicode.so",
    shared_library = "lib/linux_64/libeaiunicode.so",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libLwCommon.so.1",
    shared_library = "lib/linux_64/libLwCommon.so.1",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libLwSvm.so.1",
    shared_library = "lib/linux_64/libLwSvm.so.1",
    target_compatible_with = ["@platforms//os:linux"],
)

cc_import(
    name = "libJtTk116.so",
    shared_library = "lib/linux_64/libJtTk116.so",
    deps = [
        ":libJt-10.6.0.so",
        ":libLwCommon.so.1",
        ":libLwSvm.so.1",
        ":libParaSupt-10.6.0.so",
        ":libeaiunicode.so",
        ":libpsbodyshop.so",
        ":libpskernel.so",
        ":libpsxttoolkit.so",
    ],
    target_compatible_with = ["@platforms//os:linux"],
)

cc_library(
    name = "lib",
    hdrs = glob(["include/JtTk/**/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [":libJtTk116.so"],
    target_compatible_with = ["@platforms//os:linux"],
)