#ifndef RENDERER_H
#define RENDERER_H

#include <DrawingWindow.h>
#include <ModelTriangle.h>
#include "Camera.h"
#include "LightingConfiguration.h"
#include <memory>

enum class ConcreteRenderer { Wireframe, Rasterised, Raytraced };

class Renderer {
public:
    virtual void draw(DrawingWindow &window, Camera &camera, const std::vector<ModelTriangle> &triangles,
                      LightingConfiguration &lighting) = 0;
    static std::unique_ptr<Renderer> getRenderer(ConcreteRenderer type);
};


#endif //RENDERER_H
