Works:

bazel run --crosstool_top=@llvm_toolchain//:toolchain //:HelloWorld

Does not work:

bazel run --extra_toolchains=@llvm_toolchain//:cc-toolchain-linux //:HelloWorld
