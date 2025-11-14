#include <iostream>

__global__ void add(int a, int b, int *c) {
    *c = a + b;
}

class Image {
public:
    Image(int width, int height) : width_(width), height_(height) {
        auto error = cudaMalloc(&device_image_data,  width * height * sizeof(float));

        error = cudaMalloc(&device_image_data,  width * height * sizeof(float));
        if(error != cudaSuccess) {
             std::cout << "Error: cudaMalloc failed" << std::endl;
       }
    }

    ~Image() {
        cudaFree(device_image_data);
    }

private:
    float *device_image_data;
    int width_;
    int height_;
};

int main() {
    int *device_data_c = nullptr;

    auto error = cudaSuccess;
    error = cudaMalloc(&device_data_c, sizeof(int));

    if(error != cudaSuccess) {
        std::cout << "Error: cudaMalloc failed" << std::endl;
        return 1;
    }

    add<<<1,1>>>(2, 7, device_data_c);

    int c;
    error = cudaMemcpy(&c, device_data_c, sizeof(int), cudaMemcpyDeviceToHost);

    if(error != cudaSuccess) {
        std::cout << "Error" << std::endl;
        return 1;
    }

    Image image{4096, 4096};

    std::cout << c << std::endl;
    cudaFree(device_data_c);
    return 0;
}
