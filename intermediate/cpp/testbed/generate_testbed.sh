#!/bin/bash

# Create a WORKSPACE file
#echo "workspace(name = 'bazel-demo')" > WORKSPACE

echo "7.0.2" > .bazelversion

# Create a MODOULE.bazel file
touch MODULE.bazel

# Create a BUILD file for a simple binary target
echo "cc_binary(
   name = 'hello_world',
   srcs = ['hello_world.cpp'],
)" > BUILD.bazel

# Create a simple C++ source file
echo "#include <iostream>

int main() {
   std::cout << \"Hello, Bazel!\" << std::endl;
   return 0;
}" > hello_world.cpp

echo "bazel-*" > .gitignore

# Run the binary
bazel run //:hello_world
