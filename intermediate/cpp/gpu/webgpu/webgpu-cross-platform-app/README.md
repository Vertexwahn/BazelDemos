# Dawn for Bazel on Windows

https://developer.chrome.com/blog/webgpu-cross-platform/
https://github.com/beaufortfrancois/webgpu-cross-platform-app

## macOS

```shell
# Create an image
bazel run //:tarball
# Run docker container and delete it on quit
docker run  -p 4059:4000 vertexwahn/emscripten_webgpu:v0.0.1
```

Visit http://localhost:4059/static/index2.html

## Ubuntu 22.04

#### Docker

```shell
# Create an image
bazel run //:tarball
# Run docker container and delete it on quit
docker run  -p 4059:4000 vertexwahn/emscripten_webgpu:v0.0.1
```

Visit http://localhost:4059/static/index2.html

### Web

```shell
bazel build --config=gcc11 -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/index-wasm/index.html
cd $bazel_genfiles/index-wasm/
python3 -m http.server
```

1. Goal:

Should run on Windows and look like this:
Do not modify `main.cpp`

![Alt text](image.png)

```mermaid
  graph TD;
      app-->webgpu_dawn;
      webgpu_dawn-->dawn_common;
      webgpu_dawn-->dawn_headers;
      webgpu_dawn-->dawn_native;
      webgpu_dawn-->dawn_platform;
      webgpu_dawn-->dawncpp_headers;
      webgpu_dawn-->libtint;
      dawn_native-->abseil-cpp;
      dawn_native-->dawn_common;
      dawn_native-->dawn_headers;
      dawn_native-->dawn_platform;
      dawn_native-->dawncpp_headers;
      dawn_native-->libtint
      app-->dawncpp;
      app-->dawn_glfw;
```
