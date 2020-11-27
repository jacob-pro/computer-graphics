#ifndef CG2020_MAIN_H
#define CG2020_MAIN_H
#include "Renderer.h"

enum class ModelPreset {
    CornellBoxTextured,
    CornellBox,
    CornellBoxSoftShadow,
    CornellBoxMirror,
    Sphere,
    Hackspace,
};

struct Model {
    Model() = default;
    std::vector<ModelTriangle> triangles;
    Camera camera = Camera(glm::vec3(), 0);
    LightingConfiguration lighting;
    ConcreteRenderer renderer = ConcreteRenderer::Raytraced;
    float movementSensitivity = 1.0;
    explicit Model(ModelPreset config);
    void renderFrame(DrawingWindow &window, void(*eventHandler)(SDL_Event event, DrawingWindow &window, Model &m));
};


#endif //CG2020_MAIN_H
