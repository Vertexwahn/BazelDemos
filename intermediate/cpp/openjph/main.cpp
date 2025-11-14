#include <iostream>

#include <openjph/ojph_arch.h>


int main() {
    std::cout << "Hello, OpenJPH!" << std::endl;
    std::cout << "OpenJPH version: " << ojph::get_cpu_ext_level() << std::endl;
    return 0;
}
