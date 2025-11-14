#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_GOOGLE_include_directive : require

#include "random.glsl"
#include "structs.glsl"


// INPUTS
layout(binding = 2) uniform Scene {
    Sphere spheres[512];
    uint sphereAmount;
} scene;

layout(location = 0) rayPayloadInEXT Payload payload;

hitAttributeEXT vec3 pointOnSphere;


// ENUMS
const uint MATERIAL_TYPE_DIFFUSE = 0;
const uint MATERIAL_TYPE_METAL = 1;
const uint MATERIAL_TYPE_REFRACTIVE = 2;

const uint TEXTURE_TYPE_SOLID = 0;
const uint TEXTURE_TYPE_CHECKERED = 1;


// METHODS
vec4 getTextureColor(const Sphere sphere);
vec3 getScatterDirection(const Sphere sphere, const vec3 normal, const bool frontFace);
bool isVectorNearZero(const vec3 vector);
bool canRefract(const vec3 vector, const vec3 normal, const float eta);
float reflectanceFactor(const vec3 vector, const vec3 normal, const float eta);


// MAIN
void main() {
    const Sphere sphere = scene.spheres[gl_PrimitiveID];

    const vec3 outwardNormal = normalize(pointOnSphere - sphere.geometry.xyz);
    const bool frontFace = dot(gl_WorldRayDirectionEXT, outwardNormal) < 0.0f;
    const vec3 normal = frontFace ? outwardNormal : -outwardNormal;

    payload.attenuation = getTextureColor(sphere).rgb;
    payload.scatterDirection = getScatterDirection(sphere, normal, frontFace);
    payload.pointOnSphere = pointOnSphere;
    payload.doesScatter = payload.scatterDirection != vec3(0.0f);
}


// TEXTURE
vec4 getTextureColor(const Sphere sphere) {
    if (sphere.textureType == TEXTURE_TYPE_SOLID) {
        return sphere.colors[0];

    } else if (sphere.textureType == TEXTURE_TYPE_CHECKERED) {
        const float size = 6.0f;
        const float sines = sin(size * pointOnSphere.x) * sin(size * pointOnSphere.y) * sin(size * pointOnSphere.z);
        return sphere.colors[sines > 0.0f ? 0 : 1];
    }

    return sphere.colors[0];
}


// MATERIAL
vec3 getDiffuseScatterDirection(const Sphere sphere, const vec3 normal) {
    vec3 scatterDirection = normal + randomUnitVector(payload.seed);

    if (isVectorNearZero(scatterDirection)) {
        scatterDirection = normal;
    }

    return scatterDirection;
}

vec3 getMetalScatterDirection(const Sphere sphere, const vec3 normal) {
    const vec3 reflectedDirection = reflect(gl_WorldRayDirectionEXT, normal);
    const vec3 fuzzDireciton = sphere.materialSpecificAttribute * randomUnitVector(payload.seed);
    const vec3 scatterDirection = normalize(reflectedDirection + fuzzDireciton);

    const bool doesScatter = dot(scatterDirection, normal) > 0.0f;
    if (!doesScatter) {
        return vec3(0.0f);
    }

    return scatterDirection;
}

vec3 getRefractiveScatterDirection(const Sphere sphere, const vec3 normal, const bool frontFace) {
    const float eta = frontFace ? (1.0f / sphere.materialSpecificAttribute) : sphere.materialSpecificAttribute;
    const bool doesRefract = canRefract(gl_WorldRayDirectionEXT, normal, eta) && reflectanceFactor(gl_WorldRayDirectionEXT, normal, eta) < randomFloat(payload.seed);

    if (doesRefract) {
        return refract(gl_WorldRayDirectionEXT, normal, eta);
    }

    return reflect(gl_WorldRayDirectionEXT, normal);
}

vec3 getScatterDirection(const Sphere sphere, const vec3 normal, const bool frontFace) {
    if (sphere.materialType == MATERIAL_TYPE_DIFFUSE) {
        return getDiffuseScatterDirection(sphere, normal);
    }

    if (sphere.materialType == MATERIAL_TYPE_METAL) {
        return getMetalScatterDirection(sphere, normal);
    }

    if (sphere.materialType == MATERIAL_TYPE_REFRACTIVE) {
        return getRefractiveScatterDirection(sphere, normal, frontFace);
    }

    return vec3(0.0f);
}


// UTILITY
bool isVectorNearZero(const vec3 vector) {
    const float s = 1e-8;
    return abs(vector.x) < s && abs(vector.y) < s && abs(vector.z) < s;
}

bool canRefract(const vec3 vector, const vec3 normal, const float eta) {
    const float cosTheta = dot(-vector, normal);
    return eta * sqrt(1.0f - cosTheta * cosTheta) <= 1.0f;
}

float reflectanceFactor(const vec3 vector, const vec3 normal, const float eta) {
    const float r = pow((1.0f - eta) / (1.0f + eta), 2.0f);
    return r + (1.0f - r) * pow(1.0f - dot(-vector, normal), 5.0f);
}
