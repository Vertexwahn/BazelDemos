#include <cuda_runtime.h>

#include <iostream>

#include "kernel.h"

// This function has been copied from https://github.com/colmap/colmap/blob/main/src/colmap/util/cuda.cc
// and is availabe under the license found in colmap.LICENSE
int GetNumCudaDevices() {
  int num_cuda_devices;
  cudaGetDeviceCount(&num_cuda_devices);
  return num_cuda_devices;
}

int main() {
    const int num_cuda_devices = GetNumCudaDevices();

    std::cout << num_cuda_devices << std::endl;

    int driver_version;
    cudaDriverGetVersion(&driver_version);
    std::cout << driver_version << std::endl;

    int runtime_version;
    cudaRuntimeGetVersion(&runtime_version);
    std::cout << runtime_version << std::endl;

    launch();
    return 0;
}
