# OpenEXR Demo

This demo shows how to create a `test.exr` using OpenEXR and [Bazel](https://bazel.build/).

## Installing Bazel

First, you need to install Bazel.

If you are using macOS and have brew installed you can simply type:

```
brew install bazel
```

Using Ubuntu you can execute:

```
sudo apt update && sudo apt install bazel
```

On Windows you can use Chocolatey as shown in the following or directly download the a release build form GitHub:

```
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
choco install bazel
```

After this the `bazel` command should be available in your terminal. The `bazel` command can be called with the parameter `version` to test if the installation has worked.

```
bazel version
```

The output should look something like this:

```
Starting local Bazel server and connecting to it...
Build label: 6.1.1
Build target: bazel-out/k8-opt/bin/src/main/java/com/google/devtools/build/lib/bazel/BazelServer_deploy.jar
Build time: Wed Mar 15 15:44:56 2023 (1678895096)
Build timestamp: 1678895096
Build timestamp as int: 1678895096
```

Please note that a proper Bazel setup might require further steps as described [in the Bazel documentation](https://bazel.build/start). In detail, on a Windows machine, you need to set up [MSYS2](https://www.msys2.org/) and install tools like git.

## Clone and Run

Once you have proper setup of Bazel you can clone the repository and compile the demo.

```shell
git clone https://github.com/Vertexwahn/BazelDemos.git
cd BazelDemos/third_party_libraries/cpp/OpenEXR
bazel run //:Demo 
```
