#include <png.h>
#include <iostream>

void readpng_version_info()
{
    fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
      PNG_LIBPNG_VER_STRING, png_libpng_ver);
  
}

int main() {
	std::cout << "Hello World!" << std::endl;
	readpng_version_info();

	readpng_init
}
