#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_GOOGLE_include_directive : require

#include "structs.glsl"


// INPUTS
layout(location = 0) rayPayloadInEXT Payload payload;


// MAIN
void main() {
    payload.doesScatter = false;
    payload.attenuation = vec3(0.7f, 0.8f, 1.0f);
    payload.scatterDirection = vec3(0.0f);
    payload.pointOnSphere = vec3(0.0f);
}
