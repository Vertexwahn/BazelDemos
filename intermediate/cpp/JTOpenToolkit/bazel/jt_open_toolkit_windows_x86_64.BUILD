load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

cc_import(
    name = "Jt-10.6.0.dll",
    shared_library = "lib/win_64_VS2022/Jt-10.6.0.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "LwCommon_1_4.dll",
    shared_library = "lib/win_64_VS2022/LwCommon_1_4.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "LwSvm_1_4.dll",
    shared_library = "lib/win_64_VS2022/LwSvm_1_4.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "ParaSupt-10.6.0.dll",
    shared_library = "lib/win_64_VS2022/ParaSupt-10.6.0.dll", 
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "plmxmlAdapterJT60.dll",
    shared_library = "lib/win_64_VS2022/plmxmlAdapterJT60.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "plmxmlExtensions.dll",
    interface_library = "lib/win_64_VS2022/plmxmlExtensions.lib",
    shared_library = "lib/win_64_VS2022/plmxmlExtensions.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "plmxmlSDK.dll",
    interface_library = "lib/win_64_VS2022/plmxmlSDK.lib",
    shared_library = "lib/win_64_VS2022/plmxmlSDK.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "psbodyshop.dll",
    shared_library = "lib/win_64_VS2022/psbodyshop.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "pskernel.dll",
    shared_library = "lib/win_64_VS2022/pskernel.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "psxttoolkit.dll",
    shared_library = "lib/win_64_VS2022/psxttoolkit.dll",
    target_compatible_with = ["@platforms//os:windows"],
)

cc_import(
    name = "JtTk116.dll",
    interface_library = "lib/win_64_VS2022/JtTk116.lib",
    shared_library = "lib/win_64_VS2022/JtTk116.dll",
    deps = [
        ":Jt-10.6.0.dll",
        ":LwCommon_1_4.dll",
        ":LwSvm_1_4.dll",
        ":ParaSupt-10.6.0.dll",
        ":plmxmlAdapterJT60.dll",
        ":plmxmlExtensions.dll",
        ":plmxmlSDK.dll",
        ":psbodyshop.dll",
        ":pskernel.dll",
        ":psxttoolkit.dll",
    ],
    target_compatible_with = ["@platforms//os:windows"],
)

cc_library(
    name = "lib",
    hdrs = glob(["include/JtTk/*.h"]),
    includes = ["include"],
    #shared_library = "C:/Program Files/Siemens/JTOpenToolkit/11_6/dev/lib/jt.lib",
    visibility = ["//visibility:public"],
    deps = [":JtTk116.dll"],
    target_compatible_with = ["@platforms//os:windows"],
)
