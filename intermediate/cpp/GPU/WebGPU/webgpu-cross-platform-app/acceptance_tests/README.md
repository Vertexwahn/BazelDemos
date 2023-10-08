# How to run acceptance test

Those test work only on a single Ubuntu 22.04 system:

![works on my machine](works_on_my_machine.webp)

## MacOS

```bash
bazel build //... # fails currently on macOS (tested on M1) - not sure why...
```

## Ubuntu 22.04

```bash
bazel test //...
```

## References

- [Bazel Web Testing Rules](https://docs-legacy.aspect.build/bazelbuild/rules_webtesting/0.3.5/readme)
