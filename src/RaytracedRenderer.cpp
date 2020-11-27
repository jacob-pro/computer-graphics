#include <RayTriangleIntersection.h>
#include <CanvasPoint.h>
#include "RaytracedRenderer.h"
#define _USE_MATH_DEFINES
#include <math.h> // NOLINT(modernize-deprecated-headers)
#include <numeric>

#define MIN_SHADOW_DISTANCE 0.01
#define REFLECTION_LIMIT 20

inline std::optional<glm::vec3> getIntersection(glm::vec3 &raySource, glm::vec3 &rayDirection, const ModelTriangle &triangle) {
    glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
    glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
    glm::vec3 SPVector = raySource - triangle.vertices[0];
    glm::mat3 DEMatrix(-rayDirection, e0, e1);
    glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;
    auto t = possibleSolution[0];
    auto u = possibleSolution[1];
    auto v = possibleSolution[2];
    if ((u >= 0.0) && (u <= 1.0) && (v >= 0.0) && (v <= 1.0) && ((u + v) <= 1.0) && (t > 0)) {
        return possibleSolution;
    }
    return std::nullopt;
}

std::optional<RayTriangleIntersection> fireRay(glm::vec3 &from, glm::vec3 &rayDirection, const std::vector<ModelTriangle> &triangles, float existingDistance = 0) {
    std::optional<RayTriangleIntersection> currentClosest;
    for (size_t i = 0; i < triangles.size(); i++) {
        auto &triangle = triangles[i];
        auto possibleIntersection = getIntersection(from, rayDirection, triangle);
        if (possibleIntersection.has_value()) {
            auto distance = possibleIntersection.value()[0];
            if (!currentClosest.has_value() || distance < currentClosest.value().distanceFromCamera) {
                auto point = from + (distance * rayDirection);
                currentClosest = RayTriangleIntersection(point, distance + existingDistance, triangle, i);
            }
        }
    }
    return currentClosest;
}

std::optional<RayTriangleIntersection> fireRayReflective(glm::vec3 &from, glm::vec3 &rayDirection, const std::vector<ModelTriangle> &triangles) {
    auto currentClosest = fireRay(from, rayDirection, triangles);
    auto reflectionCount = 0;
    while (currentClosest.has_value() && currentClosest.value().intersectedTriangle.mirror) {
        reflectionCount++;
        if (reflectionCount > REFLECTION_LIMIT) {
            return std::nullopt;
        }
        auto N = currentClosest->intersectedTriangle.faceNormal();
        auto reflectionVector = rayDirection - (2.0f * N * glm::dot(rayDirection, N));
        currentClosest = fireRay(currentClosest->intersectionPoint, reflectionVector, triangles, currentClosest->distanceFromCamera);
    }
    return currentClosest;
}

bool pointCanSeeSingleLightPoint(glm::vec3 &point, glm::vec3 &light, const std::vector<ModelTriangle> &triangles, size_t surfaceTriangle) {
    auto shadowRay = light - point;
    auto shadowRayLength = glm::length(shadowRay);
    for (size_t i = 0; i < triangles.size(); i++) {
        if (i == surfaceTriangle) continue;
        auto &triangle = triangles[i];
        auto possibleIntersection = getIntersection(point, shadowRay, triangle);
        if (possibleIntersection.has_value()) {
            auto distance = possibleIntersection.value()[0];
            if (distance > MIN_SHADOW_DISTANCE && distance < shadowRayLength) {
                return false;
            }
        }
    }
    return true;
}

float pointCanSeeLightCluster(glm::vec3 &point, std::vector<glm::vec3> lightPoints, const std::vector<ModelTriangle> &triangles, size_t surfaceTriangle) {
    size_t counter = 0;
    for (auto &lp: lightPoints) {
        if (pointCanSeeSingleLightPoint(point, lp, triangles, surfaceTriangle)) counter++;
    }
    return (float) counter / (float) lightPoints.size();
}

struct LightingMethod {
    uint32_t weight;
    virtual double compute(glm::vec3 point, glm::vec3 &normal, LightingConfiguration &lighting, Camera &camera) const = 0;
    explicit LightingMethod(uint32_t weight): weight(weight) {}
};

struct ProximityLighting: public LightingMethod {
    explicit ProximityLighting(uint32_t weight): LightingMethod(weight){};
    double compute(glm::vec3 point, glm::vec3 &normal, LightingConfiguration &lighting, Camera &camera) const override {
        auto distance = glm::length(point - lighting.lightPosition);
        auto intensity = lighting.lightStrength / (4 * M_PI * std::pow(distance, 2));
        return (intensity > 1.0) ? 1.0 : intensity;
    }
};

struct AngleOfIncidenceLighting: public LightingMethod {
    explicit AngleOfIncidenceLighting(uint32_t weight): LightingMethod(weight){};
    double compute(glm::vec3 point, glm::vec3 &normal, LightingConfiguration &lighting, Camera &camera) const override {
        auto lightRay =  glm::normalize(point - lighting.lightPosition);
        auto x = glm::dot(normal, lightRay);
        return (x < 0) ? 0 : x;
    }
};

struct SpecularLighting: public LightingMethod {
    explicit SpecularLighting(uint32_t weight): LightingMethod(weight){};
    double compute(glm::vec3 point, glm::vec3 &normal, LightingConfiguration &lighting, Camera &camera) const override {
        auto lightRay =  glm::normalize(point - lighting.lightPosition);
        auto reflectionVector = lightRay - (2.0f * normal * glm::dot(lighting.lightPosition, normal));
        auto viewVector = glm::normalize(point - camera.position);
        auto x = std::pow(glm::dot(glm::normalize(reflectionVector), viewVector), lighting.specularExponent);
        return (x < 0) ? 0 : x;
    }
};

double computeBrightness(glm::vec3 point, ModelTriangle &triangle, LightingConfiguration &lighting, Camera &camera) {
    double brightness = 0;
    auto lightingMethods = std::vector<std::unique_ptr<LightingMethod>>();
    if (lighting.proximityLighting) {
        lightingMethods.push_back(std::make_unique<ProximityLighting>(lighting.proximityWeighting));
    }
    if (lighting.specularLighting) {
        lightingMethods.push_back(std::make_unique<SpecularLighting>(lighting.specularWeighting));
    }
    if (lighting.angleOfIncidenceLighting) {
        lightingMethods.push_back(std::make_unique<AngleOfIncidenceLighting>(lighting.angleOfIncidenceWeighting));
    }
    uint32_t totalWeight = std::accumulate(lightingMethods.begin(), lightingMethods.end(), 0,
                                           [](int i, const auto& o){ return o->weight + i; });
    for(const auto& i: lightingMethods) {
        double result;
        switch (lighting.shadingMode) {
            case ShadingMode::None: {
                auto faceNormal = triangle.faceNormal();
                result = i->compute(point, faceNormal, lighting, camera);
                break;
            }
            case ShadingMode::Gouraud: {
                auto a = i->compute(point, triangle.vertexNormals[0], lighting, camera);
                auto b = i->compute(point, triangle.vertexNormals[1], lighting, camera);
                auto c = i->compute(point, triangle.vertexNormals[2], lighting, camera);
                auto bary = triangle.toBarycentric(point);
                result = (a * bary[0]) + (b * bary[1]) + (c * bary[2]);
                break;
            }
            case ShadingMode::Phong: {
                auto bary = triangle.toBarycentric(point);
                auto ns = triangle.vertexNormals;
                auto avgNormal = (ns[0] * bary[0]) + (ns[1] * bary[1]) + (ns[2] * bary[2]);
                result = i->compute(point, avgNormal, lighting, camera);
                break;
            }
        }
        auto weight = (double) i->weight / (double) totalWeight;
        brightness = brightness + (weight * result);
    }
    return brightness;
}

void RaytracedRenderer::draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle> &triangles,
                             LightingConfiguration &lighting) {

    for (size_t y = 0; y < window.height; y++) {
        for (size_t x = 0; x < window.width; x++) {
            auto px = (x - window.width / 2.0);
            auto py = (y - window.height / 2.0);
            auto pixelPoint = glm::vec3 {px, py, camera.focal_length};
            auto ray = (camera.position - pixelPoint) * inverse(camera.orientation);

            auto closestIntersection = fireRayReflective(camera.position, ray, triangles);
            if (closestIntersection.has_value()) {
                auto val = closestIntersection.value();
                auto colour = val.intersectedTriangle.colour;
                colour.brightness = 0;

                if (lighting.softShadow) {
                    auto cluster = lighting.getPointCluster(val.intersectionPoint);
                    auto coverage = pointCanSeeLightCluster(val.intersectionPoint, cluster, triangles, val.triangleIndex);
                    colour.brightness = computeBrightness(val.intersectionPoint, val.intersectedTriangle, lighting, camera);
                    colour.brightness *= coverage;

                } else {
                    if (pointCanSeeSingleLightPoint(val.intersectionPoint, lighting.lightPosition, triangles, val.triangleIndex)) {
                        colour.brightness = computeBrightness(val.intersectionPoint, val.intersectedTriangle, lighting, camera);
                    }
                }
                if (lighting.ambientLighting) {
                    colour.brightness = std::max(colour.brightness, lighting.ambientThreshold);
                }

                window.setPixelColour(x, y, colour.sdlValue());
            }
        }
    }
}
