#include "fmt/core.h"

#include <iostream>

int main() {
    std::string message = fmt::format("The answer is {}.", 42);

    std::cout << message << std::endl;
    
    return 0;
}