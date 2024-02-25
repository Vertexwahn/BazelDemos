#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    #ifdef __GNUC__
        std::cout << "Hello from GCC!" << std::endl;
    #endif

    #ifdef __llvm__
        std::cout << "Hello from LLVM" << std::endl;
    #endif

    return 0;
}
