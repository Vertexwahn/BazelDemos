**How to run?**

Unfortunatelly the rules_donet are not compatible with sandboxing. Particularly, running dotnet rules on Linux or OSX requires passing --spawn_strategy=standalone.

    bazel run :HelloWorld --spawn_strategy=standalone