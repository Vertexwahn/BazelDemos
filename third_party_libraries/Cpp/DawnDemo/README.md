<!--
SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

# Legal Notes

This project contains code files copied from https://github.com/google/skia which is available under the BSD-3-Clause license. See file `LICENSE.skia`. The following files where copied from the skia project:

- `bazel/dawn.BUILD`
- `bazel/spirv_cross.BUILD`
- `bazel/vulkan_headers.BUILD`
- `bazel/vulkan_tools.BUILD`
- `requirements.txt`

This project contains code files copied from https://github.com/cwoffenden/hello-webgpu which is available under the Creative Commons Zero license (or Public Domain, whichever is applicable in your jurisdiction) license:

- `ems/webgpu.cpp`
- `ems/window.cpp`
- `qt/webgpu.cpp`
- `main.cpp`
- `webgpu.h`
- `window.h`

Modification done to this files are availabe under the same lincese that the file had before modification.

# How to run

## Ubuntu 22.04

```shell
sudo apt install google-chrome-unstable
google-chrome-unstable --enable-unsafe-webgpu --enable-features=Vulkan,UseSkiaRenderer
```

```shell
cd ${HOME}/dev/Piper/BazelDemos/third_party_libraries/cpp/DawnDemo
bazel build --config=gcc11 -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
cd $bazel_genfiles/index-wasm
sudo cp ${HOME}/dev/Piper/BazelDemos/third_party_libraries/cpp/DawnDemo/index.html index.html
python3 -m http.server
```

# Screenshots

![Screenshot](docs/ubuntu22_chrome.png)
