

# Emscripten OpenGL Demo

> Note: This demo is based on the blog post
 "WebGL with Emscripten: The Bare Minimum"
 which can be found at https://blog.feather.systems/WebGL.html

## Ubuntu 22.04

```shell
bazel build --config=gcc11 -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/index-wasm/index.html # sudo required!
cd $bazel_genfiles/index-wasm/
python3 -m http.server
```

## macOS

```shell
bazel build --config=macos -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/index-wasm/index.html # sudo required!
cd $bazel_genfiles/index-wasm/
python3 -m http.server
```

![](macOS_safai.png)

## References

- [WebGL with Emscripten: The Bare Minimum](https://blog.feather.systems/WebGL.html)
- [Porting a C++ Rendering Engine to WebAssembly](https://medium.com/cyberbotics/porting-a-c-rendering-engine-to-webassembly-9c32d76c31f1)
