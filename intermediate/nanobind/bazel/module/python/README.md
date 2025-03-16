# Python toolchain

This directory defines the Python toolchain.

Currently Python `3.13.1` is used.

## Updating the lock files

You can regenerate the lock files by running:

```shell
bazel run //bazel/module/python:requirements.update
```

This as to be repeated on every platform.

## References

- https://github.com/sschaetz/bazel-textual-experiments/
- https://github.com/bazelbuild/rules_python/blob/main/examples/pip_parse/MODULE.bazel
