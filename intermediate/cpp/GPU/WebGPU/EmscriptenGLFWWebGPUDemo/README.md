# Emscripten WebGPU Demo

## macOS

```shell
bazel build --config=macos -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/index-wasm/index.html
cd $bazel_genfiles/index-wasm/
python3 -m http.server
```

## Ubuntu 22.04

### Desktop

```shell
bazel run --config=gcc11 -- //:main # Does currently not work...
```

### Web

```shell
bazel build --config=gcc11 -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/index-wasm/index.html
cd $bazel_genfiles/index-wasm/
python3 -m http.server
```

-> Works only in Google Chrome unstable

## Windows

```shell
bazel build --config=gcc11 -- //:index-wasm
$bazel_genfiles = Invoke-Expression "bazel info bazel-genfiles"
cp -Force index.html $bazel_genfiles/index-wasm/index.html
cd $bazel_genfiles/index-wasm/
python3 -m http.server
```

## References

- [https://developer.chrome.com/blog/webgpu-cross-platform/](https://developer.chrome.com/blog/webgpu-cross-platform/)
