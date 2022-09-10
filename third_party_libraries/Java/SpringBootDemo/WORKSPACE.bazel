load('@bazel_tools//tools/build_defs/repo:http.bzl', 'http_archive')

RULES_JVM_EXTERNAL_TAG = '4.1'
RULES_JVM_EXTERNAL_SHA = 'f36441aa876c4f6427bfb2d1f2d723b48e9d930b62662bf723ddfb8fc80f0140'

http_archive(
  name = 'rules_jvm_external',
  strip_prefix = 'rules_jvm_external-%s' % RULES_JVM_EXTERNAL_TAG,
  sha256 = RULES_JVM_EXTERNAL_SHA,
  url = 'https://github.com/bazelbuild/rules_jvm_external/archive/%s.zip' % RULES_JVM_EXTERNAL_TAG,
)

load('@rules_jvm_external//:defs.bzl', 'maven_install')

maven_install(
  artifacts = [
    'org.springframework.boot:spring-boot-autoconfigure:2.1.3.RELEASE',
    'org.springframework.boot:spring-boot-starter-web:2.1.3.RELEASE',
    'org.springframework.boot:spring-boot:2.1.3.RELEASE',
    'org.springframework:spring-web:5.1.5.RELEASE',
  ],
  repositories = [
    'https://repo1.maven.org/maven2',
  ],
  fetch_sources = True,
)