#pragma once

#include <iostream>

template<typename ScalarType>
class Vector3 {
public:
	ScalarType x, y, z;

	void print() {
		#ifdef FOO_BAR
			std::cout << "FOO_BAR defined" << std::endl;
		#endif
	}
};