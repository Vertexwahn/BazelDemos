**How to run?**

Unfortunately the `rules_donet`` are not compatible with sandboxing. 
Particularly, running dotnet rules on Linux or OSX requires passing --spawn_strategy=standalone.

```shell
bazel run :HelloWorld --spawn_strategy=standalone
```
