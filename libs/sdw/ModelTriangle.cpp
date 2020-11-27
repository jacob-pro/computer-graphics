#include "ModelTriangle.h"

ModelTriangle::ModelTriangle() = default;

ModelTriangle::ModelTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, Colour trigColour) :
        vertices({{v0, v1, v2}}), texturePoints(), colour(std::move(trigColour)) {}

std::ostream &operator<<(std::ostream &os, const ModelTriangle &triangle) {
    os << "(" << triangle.vertices[0].x << ", " << triangle.vertices[0].y << ", " << triangle.vertices[0].z << ")\n";
    os << "(" << triangle.vertices[1].x << ", " << triangle.vertices[1].y << ", " << triangle.vertices[1].z << ")\n";
    os << "(" << triangle.vertices[2].x << ", " << triangle.vertices[2].y << ", " << triangle.vertices[2].z << ")\n";
    return os;
}

glm::vec3 ModelTriangle::faceNormal() {
    auto e0 = vertices[1] - vertices[0];
    auto e1 = vertices[2] - vertices[0];
    return glm::normalize(glm::cross(e0, e1));
}

//https://users.csc.calpoly.edu/~zwood/teaching/csc471/2017F/barycentric.pdf
glm::vec3 ModelTriangle::toBarycentric(glm::vec3 point) {
    auto n = glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);
    auto na = glm::cross((vertices[2] - vertices[1]), (point - vertices[1]));
    auto nb = glm::cross((vertices[0] - vertices[2]), (point - vertices[2]));
    auto nc = glm::cross((vertices[1] - vertices[0]), (point - vertices[0]));
    double n2 = std::pow(glm::length(n), 2);
    auto alpha = glm::dot(n, na) / n2;
    auto beta = glm::dot(n, nb) / n2;
    auto upsilon = glm::dot(n, nc) / n2;
    return glm::vec3{alpha, beta, upsilon};
}
