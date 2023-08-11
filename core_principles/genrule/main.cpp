/*
 *  SPDX-FileCopyrightText: 2022 Julian Amann <dev@vertexwahn.de>
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "foo.h"
#include "foo2.h"
#include "weights/foo3.h"

#include <iostream>

int main() {
#ifdef FOO
	std::cout << "FOO is defined" << std::endl;
#endif

#ifdef FOO2
	std::cout << "FOO2 is defined" << std::endl;
#endif

#ifdef FOO3
	std::cout << "FOO3 is defined" << std::endl;
#endif

#ifdef FOO4
	std::cout << "FOO3 is defined" << std::endl;
#endif
}
