# Emscripten WebGPU Demo

    bazel build --config=macos -- //:index-wasm
    bazel_genfiles=$(bazel info bazel-genfiles)
    cp index.html $bazel_genfiles/index-wasm/index.html
    cd $bazel_genfiles/index-wasm/
    python3 -m http.server