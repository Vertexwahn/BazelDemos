#include "kernel.h"

#include <cuda_runtime.h>

#include <iostream>

int cuda_device_count() {
    int num_cuda_devices{0};
    cudaGetDeviceCount(&num_cuda_devices);
    return num_cuda_devices;
}

int main() {
    const int num_cuda_devices = cuda_device_count();

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
