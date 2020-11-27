#ifndef LIGHTCONFIGURATION_H
#define LIGHTCONFIGURATION_H

#include "glm/glm.hpp"
#include <vector>

enum class ShadingMode {
    None,
    Gouraud,
    Phong
};

struct LightingConfiguration {
    LightingConfiguration() = default;
    LightingConfiguration(glm::vec3 lightPosition, float lightStrength, float lightRadius, uint32_t specularExponent);
    glm::vec3 lightPosition{};
    bool proximityLighting = true;
    uint32_t proximityWeighting = 70;
    float lightStrength = 1.0;
    bool angleOfIncidenceLighting = true;
    uint32_t angleOfIncidenceWeighting = 20;
    bool specularLighting = true;
    uint32_t specularExponent = 16;
    uint32_t specularWeighting = 10;
    bool ambientLighting = true;
    double ambientThreshold = 0.3;
    ShadingMode shadingMode = ShadingMode::None;
    bool softShadow = false;
    float lightRadius = 0.1;
    uint32_t lightPoints = 100;
    float lightPointsSample = 1;

    [[nodiscard]] std::vector<glm::vec3> getPointCluster(glm::vec3 lookingFrom) const;
};


#endif //LIGHTCONFIGURATION_H
