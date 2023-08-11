#include <string>
#include <iostream>
#include <tbb/tbb.h>

using namespace tbb;

int g_call_times = 0;

void Foo(float x) {
  g_call_times++;
}

void ParallelApplyFoo( float* a, size_t n ) {
    parallel_for( blocked_range<size_t>(0,n),
                  [=](const blocked_range<size_t>& r) {
                      for(size_t i=r.begin(); i!=r.end(); ++i)
                          Foo(a[i]);
                  }
    );
}

int main() {
    float a[3] = {1.f, 2.f, 3.f};

    ParallelApplyFoo(a, 3);

    std::cout << g_call_times << std::endl;

    return 0;
}
