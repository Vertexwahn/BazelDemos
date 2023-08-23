cd ${HOME}/dev/Piper/BazelDemos/intermediate/cpp/DawnDemo
bazel build --config=macos -- //:index-wasm
bazel_genfiles=$(bazel info bazel-genfiles)
cd $bazel_genfiles/index-wasm
sudo cp ${HOME}/dev/Piper/BazelDemos/intermediate/cpp/DawnDemo/index.html index.html
python3 -m http.server
