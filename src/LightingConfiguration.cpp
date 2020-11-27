#include <algorithm>
#include <iterator>
#include <random>
#include "LightingConfiguration.h"

LightingConfiguration::LightingConfiguration(glm::vec3 lightPosition, float lightStrength, float lightRadius, uint32_t specularExponent)
: lightPosition(lightPosition), lightStrength(lightStrength), lightRadius(lightRadius), specularExponent(specularExponent) {
}

std::vector<glm::vec3> LightingConfiguration::getPointCluster(glm::vec3 lookingFrom) const {
    // Currently representing the light as a square instead of a circle
    // The light also isn't perfectly centred in the square
    // However it shouldn't really make much difference

    uint32_t pointsInRow = floor(sqrt(lightPoints));
    auto spacing = (lightRadius * 2.0) / pointsInRow;

    // Create a light "square" around origin (0,0,0) in the X and Y plane.
    auto cluster = std::vector<glm::vec3>();
    for (uint32_t i = 0; i < pointsInRow; i++) {
        for (uint32_t j = 0; j < pointsInRow; j++) {
            auto x = 0 - lightRadius + (spacing * i);
            auto y = 0 - lightRadius + (spacing * j);
            cluster.emplace_back(x, y, 0);
        }
    }

    // Rotate the light "square" to be facing the observation point
    // https://stackoverflow.com/questions/23692077/rotate-object-to-face-point
    // https://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation#Rotating_a_vector
    // (This rotation happens around the origin)
    if (lookingFrom != lightPosition) {
        auto currentDirection = glm::vec3 {0, 0, 1};
        auto newDirection = lookingFrom - lightPosition;
        auto angle = float(acos(glm::dot(currentDirection, newDirection)));
        auto axis = glm::normalize(glm::cross(currentDirection, newDirection));
        for (auto &v : cluster) {
            auto a = float(cos(angle)) * v;
            auto b = float(sin(angle)) * glm::cross(axis, v);
            auto c = (float) (1.0 - cos(angle)) * glm::dot(axis, v) * axis;
            v = a + b + c;
        }
    }

    // Translate the light square back to the light position
    for (auto &v : cluster) {
        v = lightPosition + v;
    }

    if (lightPointsSample < 1 && lightPointsSample > 0) {
        // Randomly sample for performance reasons http://www.raytracerchallenge.com/bonus/area-light.html
        uint32_t sampleSize = lightPointsSample * cluster.size();
        auto sampledCluster = std::vector<glm::vec3>();
        std::sample(cluster.begin(), cluster.end(), std::back_inserter(sampledCluster),
                    sampleSize, std::mt19937{std::random_device{}()});
        return sampledCluster;
    }
    return cluster;
}
