# GoogleTest Demo

## Windows

```shell
bazel test --config=vs2022 //...
```

## macOS

```shell
bazel test --config=macos //...
```

## Ubuntu 22.04

```shell
bazel test --config=gcc11 //...
```

With bzlmod:

```shell
bazel test --enable_bzlmod --config=gcc11 //...
```
