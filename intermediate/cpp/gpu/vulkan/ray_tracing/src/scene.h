#pragma once

#include <glm/glm.hpp>

enum MaterialType {
    DIFFUSE = 0,
    METAL = 1,
    REFRACTIVE = 2
};

enum TextureType {
    SOLID = 0,
    CHECKERED = 1
};

struct Sphere {
    alignas(16) glm::vec4 geometry;
    alignas(4) uint32_t materialType;
    alignas(4) uint32_t textureType;
    alignas(16) glm::vec4 colors[2];
    alignas(4) float materialSpecificAttribute;
};

const uint32_t MAX_SPHERE_AMOUNT = 512;

struct Scene {
    alignas(64) Sphere spheres[MAX_SPHERE_AMOUNT];
    alignas(4) uint32_t sphereAmount;
};


Scene generateRandomScene();
