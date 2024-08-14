#include <iostream>
#include <string>
#include <sstream>

std::string ver_string(int a, int b, int c) {
  std::ostringstream ss;
  ss << a << '.' << b << '.' << c;
  return ss.str();
}

std::string true_cxx =
#ifdef __clang__
   "clang++";
#else
   "g++";
#endif

  std::string true_cxx_ver =
#ifdef __clang__
    ver_string(__clang_major__, __clang_minor__, __clang_patchlevel__);
#else
    ver_string(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::cout << "true_cxx: " << true_cxx << std::endl;
    std::cout << "true_cxx_ver: " << true_cxx_ver << std::endl;
    return 0;
}
