 The command

    bazel test --flaky_test_attempts=4 --cache_test_results=false //...

Can give the following output

    INFO: Elapsed time: 0.324s, Critical Path: 0.21s
    INFO: 2 processes: 2 linux-sandbox.
    INFO: Build completed successfully, 2 total actions
    //:tests                                                                  FLAKY, failed in 1 out of 2 in 0.1s
    Stats over 2 runs: max = 0.1s, min = 0.1s, avg = 0.1s, dev = 0.0s
    /home/user/.cache/bazel/_bazel_user/3a51ba5cfc625af4afbf63d11974cd47/execroot/GTestDemo/bazel-out/k8-fastbuild/testlogs/tests/test_attempts/attempt_1.log

    INFO: Build completed successfully, 2 total actions
