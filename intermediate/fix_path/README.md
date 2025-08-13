Based on the question from https://stackoverflow.com/questions/75188144/change-test-execution-directory-in-bazel
and
https://stackoverflow.com/questions/57536049/bazel-reading-a-file-with-relative-path-to-package-not-workspace

bazel test --cache_test_results=no --test_output=streamed //foo