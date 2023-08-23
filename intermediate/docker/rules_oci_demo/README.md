# rules_oci Demo

```shell
# List all images
docker images # or docker image ls
# Create an image
bazel run //:tarball
# Run docker container and delete it on quit
docker run vertexwahn/my_example:v0.0.1
```

## References

- https://stackoverflow.com/questions/76928215/how-to-use-rules-oci-to-run-an-image-local-via-docker
