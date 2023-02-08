def generate_cpp_from_blob_cc_library(name, **kwargs):
    native.genrule(
        name = "%s_weights_gen" % name,
        srcs = ["weights/" + name],
        outs = [
            "weights/" + name[0:-4] + ".cpp",
            "weights/" + name[0:-4] + ".h",
        ],
        cmd = "./$(location //:blob_to_cpp) weights/%s -o weights/%s.cpp -H weights/%s.h" % (name, name[0:-4], name[0:-4]),
        tools = ["//:blob_to_cpp"],
    )
    native.cc_library(
        name = name,
        srcs = ["weights/" + name[0:-4] + ".cpp"],
        hdrs = ["weights/" + name[0:-4] + ".h"],
        **kwargs
    )
