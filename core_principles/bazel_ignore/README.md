# Bazel: Ignore file

By creating a `.bazelignore` file and adding folder names releative to the Bazel workspace folders can be ignored.

```
bazel query //...
```

Displays only `//package1:HelloWorld` but not `//package2:bar`.
