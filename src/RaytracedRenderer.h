#ifndef RAYTRACEDRENDERER_H
#define RAYTRACEDRENDERER_H

#include "Renderer.h"

class RaytracedRenderer: public Renderer {
public:
    void
    draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle> &triangles, LightingConfiguration &lighting) override;
};

#endif //RAYTRACEDRENDERER_H
