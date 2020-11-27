#include "CanvasTriangle.h"
#include <algorithm>

CanvasTriangle::CanvasTriangle(CanvasPoint x, CanvasPoint y, CanvasPoint z) :
        lines({
                      LineSegment(x, y),
                      LineSegment(y, z),
                      LineSegment(z, x)
              }),
        vertices({x, y, z})
{}

CanvasTriangle::CanvasTriangle(std::array<CanvasPoint, 3> vertices):
        lines({
                      LineSegment(vertices[0], vertices[1]),
                      LineSegment(vertices[1], vertices[2]),
                      LineSegment(vertices[2], vertices[0])
              }),
        vertices(vertices) {
}

const std::array<LineSegment, 3> &CanvasTriangle::getLines() const {
    return lines;
}

const std::optional<Colour> &CanvasTriangle::getLineColour() const {
    return lineColour;
}

void CanvasTriangle::setLineColour(const std::optional<Colour> &lineColour) {
    this->lineColour = lineColour;
    for(LineSegment &l: lines) {
        if (lineColour.has_value()) {
            l.setColour(lineColour.value());
        } else {
            l.setColour(Colour());
        }
    }
}

const std::optional<Colour> &CanvasTriangle::getFillColour() const {
    return fillColour;
}

void CanvasTriangle::setFillColour(const std::optional<Colour> &fillColour) {
    this->fillColour = fillColour;
}

const std::array<CanvasPoint, 3> &CanvasTriangle::getVertices() const {
    return vertices;
}

const std::optional<TextureMap> &CanvasTriangle::getTextureMap() const {
    return textureMap;
}

void CanvasTriangle::setTextureMap(const std::optional<TextureMap> &textureMap) {
    this->textureMap = textureMap;
}

std::map<int32_t, std::vector<int32_t>> CanvasTriangle::mapYsToXs() const {
    auto x = std::map<int32_t, std::vector<int32_t>>();
    for(auto &line: lines) {
        auto xs = line.mapYsToXs();
        for(const auto &v: xs) {
            auto &a = x[v.first];
            a.insert(std::end(a), std::begin(v.second), std::end(v.second));
        }
    }
    return x;
}

//https://users.csc.calpoly.edu/~zwood/teaching/csc471/2017F/barycentric.pdf
//https://stackoverflow.com/a/23980897/7547647
glm::vec3 CanvasTriangle::computeBarycentricCoordinates(float x, float y) const {
    auto x1 = vertices[0].x;
    auto x2 = vertices[1].x;
    auto x3 = vertices[2].x;
    auto y1 = vertices[0].y;
    auto y2 = vertices[1].y;
    auto y3 = vertices[2].y;

    auto l1_num = (y2-y3)*(x-x3) + (x3-x2)*(y-y3);
    auto l1_den = (y2-y3)*(x1-x3) + (x3-x2)*(y1-y3);
    auto l1 = l1_num / l1_den;

    auto l2_num = (y3-y1)*(x-x3) + (x1-x3)*(y-y3);
    auto l2_den = (y2-y3)*(x1-x3) + (x3-x2)*(y1-y3);
    auto l2 = l2_num / l2_den;

    return glm::vec3{l1, l2, (1.0 - l1 - l2)};
}

CanvasPoint convert_point(const glm::vec3 &coords, float focal_length, TexturePoint texturePoint, float width, float height) {
    auto u = (focal_length * coords[0] / coords[2]) + (width / 2.0f);
    auto v = (focal_length * coords[1] / coords[2]) + (height / 2.0f);
    auto p = CanvasPoint(u, v);
    p.depth = -coords[2];
    p.texturePoint = texturePoint;
    return p;
}

CanvasTriangle CanvasTriangle::fromModelTriangle(const ModelTriangle &triangle, Camera &camera,
                                                 float canvas_width, float canvas_height) {
    auto points = std::array<CanvasPoint, 3>();
    for (size_t i = 0; i < 3; i++) {
        auto vertex = (triangle.vertices[i] - camera.position) * camera.orientation;
        auto texturePoint = triangle.texturePoints[i];
        points[i] = convert_point(vertex, camera.focal_length, texturePoint, canvas_width, canvas_height);
    }
    auto newTriangle = CanvasTriangle(points);
    newTriangle.setFillColour(triangle.colour);
    if (triangle.textureMap.has_value()) {
        newTriangle.setTextureMap(triangle.textureMap);
    }
    return newTriangle;
}

bool CanvasTriangle::isInBox(int32_t start_x, int32_t end_x, int32_t start_y, int32_t end_y) const {
    bool x_cross = true;
    auto min_x = (int32_t) std::min({vertices[0].x, vertices[1].x, vertices[2].x});
    auto max_x = (int32_t) std::max({vertices[0].x, vertices[1].x, vertices[2].x});
    if (max_x < start_x || min_x >= (int32_t) end_x) {
        x_cross = false;
    }
    bool y_cross = true;
    auto min_y = (int32_t) std::min({vertices[0].y, vertices[1].y, vertices[2].y});
    auto max_y = (int32_t) std::max({vertices[0].y, vertices[1].y, vertices[2].y});
    if (max_y < start_y || min_y >= (int32_t) end_y) {
        y_cross = false;
    }
    return x_cross && y_cross;
}

std::ostream &operator<<(std::ostream &os, const CanvasTriangle &triangle) {
    os << triangle.vertices[0] << triangle.vertices[1] << triangle.vertices[2];
    return os;
}
