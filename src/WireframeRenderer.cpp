#include <CanvasTriangle.h>
#include "WireframeRenderer.h"

void WireframeRenderer::draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle> &triangles,
                             LightingConfiguration &lighting) {
    for (auto &t: triangles) {
        auto converted = CanvasTriangle::fromModelTriangle(t, camera, (float) window.width, (float) window.height);
        converted.setLineColour(Colour(255,255,255));
        for(auto &line: converted.getLines()) {
            line.draw(window);
        }
    }
}
