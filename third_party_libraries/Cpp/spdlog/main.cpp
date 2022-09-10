#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <iostream>

int main() {
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
}
