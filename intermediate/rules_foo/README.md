# Readme

## How to test?

```shell
cd tests
bazel run //:hello_world
```

## Story

Let us assume there is a binary that comes for all targeted OS versions called PI generator.
The code of this tool is not available - only binaries.

## References

- https://jayconrod.com/posts/106/writing-bazel-rules-simple-binary-rule
- https://john-millikin.com/bazel-school/rules
- https://bazel.build/extending/toolchains

## Steps to success

Version 1: Let Starlark generate the header file
Version 2: Use an one cc_binary or go_binary to genrate the header file
Version 3: Move compiler outside of the repo and fetch it