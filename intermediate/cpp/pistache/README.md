# Readme

openapi-generator-cli generate -i hello_world.yaml -g cpp-pistache-server -o ./hello_world

bazel run --config=gcc11 //:server
docker run -p 8080:8080  bazel:server

curl -X 'GET' \
  'localhost:8080/hello-service/v1/hello' \
  -H 'accept: application/json'
  
## References

https://github.com/OpenAPITools/openapi-generator-bazel
https://www.dev-insider.de/schlanke-microservices-mit-pistache-a-87155e2f183e637103e19708200f8931/
https://github.com/SashaNullptr/Cpp-Server/blob/master/WORKSPACE
https://damuliu.medium.com/openapi-code-generation-bazel-and-spring-boot-e7a3603b3289

