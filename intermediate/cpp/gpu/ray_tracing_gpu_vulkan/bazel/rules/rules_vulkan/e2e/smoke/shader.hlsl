#include "common/common.h"

[[vk::binding(0)]]
RWStructuredBuffer<uint> OutputBuffer : register(u0);

[numthreads(1, 1, 1)]
void CSMain(uint3 id: SV_DispatchThreadID) { OutputBuffer[id.x] = VALUE; }

