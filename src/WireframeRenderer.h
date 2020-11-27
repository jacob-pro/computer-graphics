#ifndef WIREFRAMERENDERER_H
#define WIREFRAMERENDERER_H

#include "Renderer.h"

class WireframeRenderer: public Renderer {
public:
    void
    draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle> &triangles, LightingConfiguration &lighting) override;
};


#endif //WIREFRAMERENDERER_H
