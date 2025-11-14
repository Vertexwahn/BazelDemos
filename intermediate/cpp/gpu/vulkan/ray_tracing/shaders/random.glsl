uint getRandomSeed(const uint val0, const uint val1) {
    uint v0 = val0;
    uint v1 = val1;
    uint s0 = 0;

    for (uint n = 0; n < 16; n++) {
        s0 += 0x9e3779b9;
        v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
        v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
    }

    return v0;
}

uint randomInt(inout uint seed) {
    seed = 1664525 * seed + 1013904223;
    return seed;
}

float randomFloat(inout uint seed) {
    return float(randomInt(seed) & 0x00FFFFFFu) / float(0x01000000u);
}

float randomInInterval(inout uint seed, const float min, const float max) {
    return randomFloat(seed) * (max - min) + min;
}

vec3 randomVector(inout uint seed, const float min, const float max) {
    return vec3(randomInInterval(seed, min, max), randomInInterval(seed, min, max), randomInInterval(seed, min, max));
}

vec3 randomUnitVector(inout uint seed) {
    return normalize(randomVector(seed, -1.0f, 1.0f));
}
