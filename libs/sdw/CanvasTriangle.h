#pragma once

#include <CanvasPoint.h>
#include "Colour.h"
#include <optional>
#include <array>
#include <TextureMap.h>
#include "../../src/LineSegment.h"
#include "glm/glm.hpp"
#include "../../src/DepthBufferedWindow.h"
#include "ModelTriangle.h"
#include "../../src/Camera.h"

class CanvasTriangle {

public:
    explicit CanvasTriangle(std::array<CanvasPoint, 3> vertices);

    CanvasTriangle(CanvasPoint x, CanvasPoint y, CanvasPoint z);

    [[nodiscard]] const std::array<CanvasPoint, 3> &getVertices() const;

    [[nodiscard]] const std::array<LineSegment, 3> &getLines() const;

    [[nodiscard]] const std::optional<Colour> &getLineColour() const;
    void setLineColour(const std::optional<Colour> &lineColour);

    [[nodiscard]] const std::optional<Colour> &getFillColour() const;
    void setFillColour(const std::optional<Colour> &fillColour);

    [[nodiscard]] const std::optional<TextureMap> &getTextureMap() const;
    void setTextureMap(const std::optional<TextureMap> &textureMap);

    [[nodiscard]] glm::vec3 computeBarycentricCoordinates(float x, float y) const;

    [[nodiscard]] std::map<int32_t, std::vector<int32_t>> mapYsToXs() const;

    [[nodiscard]] bool isInBox(int32_t start_x, int32_t end_x, int32_t start_y, int32_t end_y) const;

    static CanvasTriangle fromModelTriangle(const ModelTriangle &triangle,
                                            Camera &camera,
                                            float canvas_width,
                                            float canvas_height);

    friend std::ostream &operator<<(std::ostream &os, const CanvasTriangle &triangle);

private:
    std::array<CanvasPoint, 3> vertices;
    std::array<LineSegment, 3> lines;
    std::optional<Colour> lineColour;
    std::optional<Colour> fillColour;
    std::optional<TextureMap> textureMap;
};

std::ostream &operator<<(std::ostream &os, const CanvasTriangle &triangle);
