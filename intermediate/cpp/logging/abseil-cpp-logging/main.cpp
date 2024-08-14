#include <iostream>

#include "absl/log/log.h"
#include "absl/log/initialize.h"

int main() {
    absl::InitializeLog();
    LOG(INFO) << "Hello from abseil!";
    return 0;
}
