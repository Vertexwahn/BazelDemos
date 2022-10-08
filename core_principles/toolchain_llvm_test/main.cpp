#include <iostream>

int main() {
#if defined(__clang__)
	std::cout << "Hello from CLang " << __clang_version__ << "!" << std::endl;
#elif defined(__GNUC__) || defined(__GNUG__)
	std::cout << "Hello from GCC!" << std::endl;
#elif defined(_MSC_VER)
	std::cout << "Hello from MSVC!" << std::endl;
#endif

	std::cout << "Hello World!" << std::endl;
}
