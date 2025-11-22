# Readme

## macOS arm64

```shell
bazel run //:tarball # Build and publish docker images
docker run -p 4000:4000 --rm vertexwahn/rules_oci_go_demo:v0.0.5 # Run docker and delete it on quit
```

Visit

- Home endpoint "/": http://localhost:4000
- Static route: http://localhost:4000/static/moo.html
