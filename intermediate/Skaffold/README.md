# How to use Skaffold with Bazel

```shell
minikube start
skaffold dev
```

If you get the error `invalid skaffold config: proxy: unknown scheme: http` try:

```shell
unset ALL_PROXY
unset all_proxy
```

## Notes

```shell
# Build docker image local
bazel run //:skaffold_example.tar
# Run docker container local
docker run skaffold_example:v0.0.1
# Skaffold
skaffold dev
```

## Copyright & License notes

Copied from https://github.com/GoogleContainerTools/skaffold/tree/main/examples/bazel
Under Apache-2.0 license
Copyright belongs to The Skaffold Authors

## References

- https://subscription.packtpub.com/book/programming/9781801077118/12/ch12lvl1sec69/implementing-a-gitops-workflow-with-argo-cd-and-skaffold
