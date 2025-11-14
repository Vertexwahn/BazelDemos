#include "scene.h"
#include <random>

float randomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(engine);
}

float randomFloat() {
    return randomFloat(0.0f, 1.0f);
}

// https://www.codespeedy.com/hsv-to-rgb-in-cpp/
glm::vec4 getRandomColor() {
    float h = std::floor(randomFloat(0.0f, 360.0f));
    float s = 0.75f, v = 0.45f;

    float C = s * v;
    float X = C * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - C;

    float r, g, b;

    if (h >= 0 && h < 60) {
        r = C, g = X, b = 0;
    } else if (h >= 60 && h < 120) {
        r = X, g = C, b = 0;
    } else if (h >= 120 && h < 180) {
        r = 0, g = C, b = X;
    } else if (h >= 180 && h < 240) {
        r = 0, g = X, b = C;
    } else if (h >= 240 && h < 300) {
        r = X, g = 0, b = C;
    } else {
        r = C, g = 0, b = X;
    }

    return {r + m, g + m, b + m, 1.0f};
}

Scene generateRandomScene() {
    Scene scene = {};

    scene.spheres[0] = {
            .geometry = glm::vec4(0.0f, -1000.0f, 1.0f, 1000.0f),
            .materialType = MaterialType::DIFFUSE,
            .textureType = TextureType::CHECKERED,
            .colors = {glm::vec4(0.05f, 0.05f, 0.05f, 1.0f), glm::vec4(0.95f, 0.95f, 0.95f, 1.0f)},
            .materialSpecificAttribute = 0.0f
    };

    scene.spheres[1] = {
            .geometry = glm::vec4(-4.0f, 1.0f, 0.0f, 1.0f),
            .materialType = MaterialType::DIFFUSE,
            .textureType = TextureType::SOLID,
            .colors = {glm::vec4(0.6f, 0.3f, 0.1f, 1.0f)},
            .materialSpecificAttribute = 0.0f
    };

    scene.spheres[2] = {
            .geometry = glm::vec4(4.0f, 1.0f, 0.0f, 1.0f),
            .materialType = MaterialType::METAL,
            .textureType = TextureType::SOLID,
            .colors = {glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)},
            .materialSpecificAttribute = 0.0f
    };

    scene.spheres[3] = {
            .geometry = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
            .materialType = MaterialType::REFRACTIVE,
            .textureType = TextureType::SOLID,
            .colors = {glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)},
            .materialSpecificAttribute = 1.5f
    };

    uint32_t sphereIndex = 4;

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            scene.spheres[sphereIndex].geometry =
                    glm::vec4(float(a) + 0.9f * randomFloat(), 0.2f, float(b) + 0.9f * randomFloat(), 0.2f);

            const float materialProbability = randomFloat();

            if (materialProbability < 0.7) {
                scene.spheres[sphereIndex].materialType = MaterialType::DIFFUSE;
                scene.spheres[sphereIndex].textureType = TextureType::SOLID;
                scene.spheres[sphereIndex].colors[0] = getRandomColor();
                scene.spheres[sphereIndex].materialSpecificAttribute = 0.0f;

            } else if (materialProbability < 0.85) {
                scene.spheres[sphereIndex].materialType = MaterialType::METAL;
                scene.spheres[sphereIndex].textureType = TextureType::SOLID;
                scene.spheres[sphereIndex].colors[0] = glm::vec4(randomFloat(0.5f, 1.0f), randomFloat(0.5f, 1.0f),
                                                                 randomFloat(0.5f, 1.0f), 1.0f);
                scene.spheres[sphereIndex].materialSpecificAttribute = 0.0f;

            } else {
                scene.spheres[sphereIndex].materialType = MaterialType::REFRACTIVE;
                scene.spheres[sphereIndex].textureType = TextureType::SOLID;
                scene.spheres[sphereIndex].colors[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                scene.spheres[sphereIndex].materialSpecificAttribute = 1.5f;
            }

            sphereIndex++;
        }
    }

    scene.sphereAmount = sphereIndex;
    return scene;
}
