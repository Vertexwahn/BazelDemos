# rules_oci demo

```shell
# List all images
docker images # or docker image ls

# ONLY NEEDED IF IMAGE ALREADY EXISTS
# delete docker image by name
docker rmi vertexwahn/my_example:v0.0.1 -f

# Create an image
bazel run //:tarball
# Run docker container and delete it on quit
docker run vertexwahn/my_example:v0.0.1
```

## References

- https://stackoverflow.com/questions/76928215/how-to-use-rules-oci-to-run-an-image-local-via-docker
