export void square(uniform float vin[], uniform float vout[],
                   uniform int count) {
    foreach (index = 0 ... count) {
        float v = vin[index];
        v = v * v;
        vout[index] = v;
    }
}
