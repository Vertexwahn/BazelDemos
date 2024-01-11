package(default_visibility = ["//visibility:public"])

MAIN_MODULES = [
    "core",
    "imgproc",
    "imgcodecs",
    "videoio",
    "highgui",
    "video",
    "calib3d",
    "features2d",
    "objdetect",
    "dnn",
    "ml",
    "flann",
    "photo",
    "stitching",
    "gapi",
]

# https://stackoverflow.com/questions/56108940/how-to-specify-the-compiler-flags-to-be-used-in-opt-compilation-mode-by-my-own
config_setting(
    name = "fastbuild_mode",
    values = {"compilation_mode": "fastbuild"},
)

config_setting(
    name = "dbg_mode",
    values = {"compilation_mode": "dbg"},
)

cc_import(
    name = "tbb",
    shared_library = select({
        ":fastbuild_mode": "bin/tbb.dll",
        ":dbg_mode": "bin/tbb_debug.dll",
        "//conditions:default": "bin/tbb.dll",
    }),
)

[
    (
        cc_import(
            name = module,
            interface_library = select({
                ":fastbuild_mode": "lib/opencv_{}430.lib".format(module),
                ":dbg_mode": "lib/opencv_{}430d.lib".format(module),
                "//conditions:default": "lib/opencv_{}430.lib".format(module),
            }),
            shared_library = select({
                ":fastbuild_mode": "bin/opencv_{}430.dll".format(module),
                ":dbg_mode": "bin/opencv_{}430d.dll".format(module),
                "//conditions:default": "bin/opencv_{}430.dll".format(module),
            }),
        )
    )
    for module in MAIN_MODULES
]

cc_library(
    name = "opencv",
    hdrs = [
        "include/opencv2/calib3d.hpp",
        "include/opencv2/calib3d/calib3d.hpp",
        "include/opencv2/calib3d/calib3d_c.h",
        "include/opencv2/core.hpp",
        "include/opencv2/core/hal/interface.h",
        "include/opencv2/cvconfig.h",
        "include/opencv2/dnn.hpp",
        "include/opencv2/features2d.hpp",
        "include/opencv2/flann.hpp",
        "include/opencv2/flann/config.h",
        "include/opencv2/flann/defines.h",
        "include/opencv2/flann/miniflann.hpp",
        "include/opencv2/highgui.hpp",
        "include/opencv2/highgui/highgui.hpp",
        "include/opencv2/highgui/highgui_c.h",
        "include/opencv2/imgcodecs.hpp",
        "include/opencv2/imgproc.hpp",
        "include/opencv2/ml.hpp",
        "include/opencv2/ml/ml.inl.hpp",
        "include/opencv2/objdetect.hpp",
        "include/opencv2/opencv.hpp",
        "include/opencv2/opencv_modules.hpp",
        "include/opencv2/photo.hpp",
        "include/opencv2/stitching.hpp",
        "include/opencv2/video.hpp",
        "include/opencv2/video/background_segm.hpp",
        "include/opencv2/video/tracking.hpp",
        "include/opencv2/videoio.hpp",
        "include/opencv2/videoio/videoio_c.h",
    ],
    includes = ["include"],
    deps = MAIN_MODULES + [
        "tbb",
    ],
)
