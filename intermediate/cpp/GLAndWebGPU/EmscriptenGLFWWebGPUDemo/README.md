# Emscripten WebGPU Demo

## macOS

```shell
bazel build --config=macos -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/index-wasm/index.html
cd $bazel_genfiles/index-wasm/
python3 -m http.server
```

## References

- [https://developer.chrome.com/blog/webgpu-cross-platform/](https://developer.chrome.com/blog/webgpu-cross-platform/)
