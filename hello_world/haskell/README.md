# Getting started with Haskell

https://stackoverflow.com/questions/1012573/getting-started-with-haskell?rq=1
->
http://www.happylearnhaskelltutorial.com/contents.html
->

## Ubuntu 18.04

Installing the following packages

```shell
sudo apt-get install build-essential libffi-dev libgmp-dev libtinfo5 libtinfo-dev python python3
```

Otherwise, the Haskell rules will not work.

## Ubuntu 22.04

Installing the following packages

```shell
sudo apt-get install build-essential libffi-dev libgmp-dev libtinfo5 libtinfo-dev python python3
```

## Windows 10

```shell
bazel build --crosstool_top=@rules_haskell_ghc_windows_amd64//:cc_toolchain //...
```
