#ifndef RASTERISEDRENDERER_H
#define RASTERISEDRENDERER_H

#include "Renderer.h"

class RasterisedRenderer: public Renderer {
public:
    void
    draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle> &triangles, LightingConfiguration &lighting) override;
};


#endif //RASTERISEDRENDERER_H
