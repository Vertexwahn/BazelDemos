<!--
SPDX-FileCopyrightText: 2022-2025 Julian Amann <dev@vertexwahn.de>
SPDX-License-Identifier: Apache-2.0
-->

[![Support Ukraine](https://img.shields.io/badge/Support-Ukraine-FFD500?style=flat&labelColor=005BBB)](https://opensource.fb.com/support-ukraine)
[![Build Status](https://vertexwahn.visualstudio.com/BazelDemos/_apis/build/status/Vertexwahn.BazelDemos?branchName=main)](https://vertexwahn.visualstudio.com/BazelDemos/_build/latest?definitionId=6&branchName=master)

# BazelDemos

[Bazel](https://bazel.build/) is a build system.
In this repository,
I store some demos, experiments, 
and playgrounds that show how to use Bazel with different programming languages, 
libraries, and tools.
Besides this, 
you can find misc experiments related to Bazel here.
Most of the things you find here are not production-ready but can serve as a starting point for your own developments.

## Organization of this repo

This repository is mainly organized into three parts:

- [core_principles](/core_principles): Core principles of Bazel with no focus on a specific programming language
- [hello_world](/hello_world): Plain Hello World demos in different programming languages supported by the Bazel build system
- [intermediate](/intermediate): Usage of different third-party dependencies and tools

Furthermore, the [ci](/ci) directory contains build jobs to ensure that specific demos work across different operating systems.

## Hello World

Under [hello_world](/hello_world) you can find Hello World examples in misc programming languages using Bazel.
The following table shows on which OS configurations the examples were successfully tested. 

| Language support on  | macOS   | Ubuntu |  Windows |
| -------------------- | ------- | ------ |  ------- |
| C++                  | yes     | yes    |  yes     |
| C#                   | yes     | yes    |  yes     |
| D                    | no      | yes    |  no      |
| Rust                 |         | yes    |          |

## Similar projects

Here you can find a list of other repositories with Bazel demos:

- [aspect-build/bazel-examples](https://github.com/aspect-build/bazel-examples)
- [bazelbuild/codelabs](https://github.com/bazelbuild/codelabs)
- [bazelbuild/examples](https://github.com/bazelbuild/examples)
- [clchiou/bazel-examples](https://github.com/clchiou/bazel-examples)
- [FaBrand/bazel-learning](https://github.com/FaBrand/bazel-learning)
- [kriscfoster/multi-language-bazel-monorepo](https://github.com/kriscfoster/multi-language-bazel-monorepo)
- [limdor/bazel-examples](https://github.com/limdor/bazel-examples)
- [salesforce/bazel-demo](https://github.com/salesforce/bazel-demo)

## Contributions welcome

If you have a Bazel demo you want to share, feel free to open a pull request.
I am always happy about seeing new programming languages supported by Bazel or interesting usage of tools or third-party dependencies. 
Of course, improvements are also welcome.

## License

If not stated otherwise the license of the source files in this repository (BazelDemos) is under the Apache License 2.0.

## Credits

A big thanks goes to all the providers, developers, and maintainers of the Bazel universe! :heart:
