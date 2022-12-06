#include "header.h"
#include <iostream>

int main() {
	Vector3<float> vec;
	vec.print();

	#ifdef FOO_BAR
			std::cout << "FOO_BAR is also known in main" << std::endl;
	#endif
}
