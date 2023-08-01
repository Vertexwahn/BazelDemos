# Spring Boot Demo

Run Spring Boot:

```shell
bazel run //:app
```

Open a second terminal an curl:

```shell
curl http://localhost:8080/greet?name=Vertexwahn
```

## References

- [Building a Spring Boot server with Bazel](https://ncona.com/2021/11/building-a-spring-boot-server-with-bazel)