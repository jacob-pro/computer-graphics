#include <CanvasTriangle.h>
#include "RasterisedRenderer.h"
#include <algorithm>

uint32_t textureMap(const glm::vec3 &bary, const TextureMap &textureMap, const std::array<CanvasPoint, 3> &vertices, double zInv) {
#ifdef DISABLE_PERSPECTIVE_CORRECTION
    auto ix = bary[0] * vertices[0].texturePoint.x + bary[1] * vertices[1].texturePoint.x + bary[2] * vertices[2].texturePoint.x;
    auto iy = bary[0] * vertices[0].texturePoint.y + bary[1] * vertices[1].texturePoint.y + bary[2] * vertices[2].texturePoint.y;
    auto px = (int32_t) round(ix * textureMap.width);
    auto py = (int32_t) round(iy * textureMap.height);
    return textureMap.get_pixel(px, py);
#else
    // https://www.gamedev.net/forums/topic/593669-perspective-correct-barycentric-coordinates/4763463/
    auto u = (bary[0] * vertices[0].texturePoint.x / vertices[0].depth) +
            (bary[1] * vertices[1].texturePoint.x / vertices[1].depth) +
            (bary[2] * vertices[2].texturePoint.x / vertices[2].depth);
    auto v = (bary[0] * vertices[0].texturePoint.y / vertices[0].depth) +
            (bary[1] * vertices[1].texturePoint.y / vertices[1].depth) +
            (bary[2] * vertices[2].texturePoint.y / vertices[2].depth);

    auto px = (int32_t) round(u * textureMap.width / zInv);
    auto py = (int32_t) round(v * textureMap.height / zInv);
    return textureMap.getPixel(px, py);
#endif
}

void RasterisedRenderer::draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle> &triangles,
                              LightingConfiguration &lighting) {
    auto buffer = DepthBufferedWindow(window);
    for (auto &triangle: triangles) {
        auto t = CanvasTriangle::fromModelTriangle(triangle, camera, (float) window.width, (float) window.height);
        // Skip triangles not in the window box
        if (!t.isInBox(0, (int32_t) buffer.getWidth(), 0, (int32_t) buffer.getHeight())) continue;
        if (t.getFillColour().has_value() || t.getTextureMap().has_value()) {
            auto &vertices = t.getVertices();
            auto minY = (int32_t) std::min({vertices[0].y, vertices[1].y, vertices[2].y});
            auto maxY = (int32_t) std::max({vertices[0].y, vertices[1].y, vertices[2].y});

            auto xs = t.mapYsToXs();
            std::map<int32_t, std::tuple<int32_t, int32_t>> map;
            for(const auto& i: xs) {
                int32_t min = *std::min_element( std::begin(i.second), std::end(i.second) );
                int32_t max = *std::max_element( std::begin(i.second), std::end(i.second) );
                map[i.first] = std::tuple(min, max);
            }

            // Skip rows that fall outside the window
            auto cappedMinY = std::max(minY, 0);
            auto cappedMaxY = std::min(maxY, (int32_t) buffer.getHeight());

            for (int32_t y = cappedMinY; y < cappedMaxY; y++) {
                auto minX = std::get<0>(map[y]);
                auto maxX = std::get<1>(map[y]);
                // Skip columns that fall outside the window
                auto cappedMinX = std::max(minX, 0);
                auto cappedMaxX = std::min(maxX, (int32_t) buffer.getWidth());
                for (int32_t x = cappedMinX; x < cappedMaxX; ++x) {

                    // https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/visibility-problem-depth-buffer-depth-interpolation
                    auto bary = t.computeBarycentricCoordinates((float) x, (float) y);
                    // Seems more accurate to do inverse of each
                    auto zInv = (1.0 / vertices[0].depth * bary[0]) + (1.0 / vertices[1].depth * bary[1]) + (1.0 / vertices[2].depth * bary[2]);

                    if (t.getTextureMap().has_value()) {
                        auto pixel = textureMap(bary, t.getTextureMap().value(), vertices, zInv);
                        buffer.setPixelColour(x, y, zInv, pixel);
                    } else if (t.getFillColour().has_value()) {
                        buffer.setPixelColour(x, y, zInv, t.getFillColour()->sdlValue());
                    }

                }
            }
        }
    }
}
