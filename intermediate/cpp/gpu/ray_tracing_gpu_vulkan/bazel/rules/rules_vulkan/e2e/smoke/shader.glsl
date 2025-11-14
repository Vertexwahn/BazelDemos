#version 450

#include "common/common.h"

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer OutputBuffer {
    uint data[];
};

void main() {
    uint idx = gl_GlobalInvocationID.x;
    data[idx] = VALUE;
}

