#pragma once

#include <glm/glm.hpp>
#include <string>
#include <array>
#include "Colour.h"
#include "TexturePoint.h"
#include <optional>
#include "TextureMap.h"

struct ModelTriangle {
    std::array<glm::vec3, 3> vertices{};
    std::array<TexturePoint, 3> texturePoints{};
    Colour colour{};
    std::optional<TextureMap> textureMap{};
    glm::vec3 faceNormal();
    std::array<glm::vec3, 3> vertexNormals{};
    bool mirror = false;

    ModelTriangle();
    ModelTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, Colour trigColour);
    friend std::ostream &operator<<(std::ostream &os, const ModelTriangle &triangle);
    glm::vec3 toBarycentric(glm::vec3 point);
};
