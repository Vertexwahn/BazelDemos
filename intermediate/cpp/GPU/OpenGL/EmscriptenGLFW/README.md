# Emscripten OpenGL+GLFW Demo

## Quick start

### macOS

#### Desktop

```shell
bazel run --config=macos //:main
```

![](docs/macOS_desktop.png)

#### Emscripten

Build emscripten variant and start a python3 web server to host it:

```shell
bazel build --config=macos -- //:emscripten_main_wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/emscripten_main_wasm/index.html # sudo
cd $bazel_genfiles/emscripten_main_wasm/
python3 -m http.server
```

Visit http://localhost:8000/index.html

![View of the EmscriptenGLFW demo in Safari on macOS](docs/macOS_safari.png)

#### Docker

```shell
# Create an image
bazel run //:tarball
# Run docker container and delete it on quit
docker run  -p 4000:4000 vertexwahn/emscripten_glfw:v0.0.1
```

Visit http://localhost:4000/static/index2.html

### Ubuntu 22.04

#### Desktop

```shell
bazel run --config=gcc11 //:main
```

#### Emscripten

```shell
bazel build --config=gcc11 -- //:emscripten_main_wasm
bazel_genfiles=$(bazel info bazel-genfiles)
sudo cp index.html $bazel_genfiles/emscripten_main_wasm/index.html # sudo
cd $bazel_genfiles/emscripten_main_wasm/
python3 -m http.server
```

Visit http://localhost:8000/index.html

#### Docker


```shell
# Create an image
bazel run //:tarball
# Run docker container and delete it on quit
docker run  -p 4000:4000 vertexwahn/emscripten_glfw:v0.0.1
```

Visit http://localhost:4000/static/index2.html

### Windows

#### Desktop

```shell
bazel run --config=vs2022 //:main
```

![Windows Desktop](docs/windows_desktop.png)

#### Web

```shell
bazel build --config=gcc11 -- //:emscripten_main_wasm
$bazel_genfiles = Invoke-Expression "bazel info bazel-genfiles"
cp -Force index.html $bazel_genfiles/emscripten_main_wasm/index.html
cd $bazel_genfiles/emscripten_main_wasm/
python3 -m http.server
```

![Windows Chrome](docs/windows_chrome.png)

## Building & Testing

### macOS

#### CLion

```
directories:
  .

derive_targets_from_directories: true

targets:
  # If source code isn't resolving, add additional targets that compile it here

additional_languages:
  python

build_flags:
  --config=macos
```
