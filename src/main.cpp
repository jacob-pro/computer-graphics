#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include "CanvasTriangle.h"
#include "ObjFile.h"
#include "main.h"
#include "AnimationDriver.h"

#define WIDTH 640
#define HEIGHT 480

Model::Model(ModelPreset config) {
    switch (config) {
        case ModelPreset::CornellBoxTextured:
            triangles = loadObj("textured-cornell-box.obj", 0.17f);
            camera = Camera(glm::vec3{0, 0, 4}, 1400);
            lighting = LightingConfiguration(glm::vec3 {0, 2, 0} * 0.17f, 4.0, 0.1, 16);
            renderer = ConcreteRenderer::Rasterised;
            break;
        case ModelPreset::CornellBox:
            triangles = loadObj("cornell-box.obj", 0.17f);
            camera = Camera(glm::vec3{0, 0, 4}, 1400);
            lighting = LightingConfiguration(glm::vec3 {0, 2, 0} * 0.17f, 4.0, 0.1, 16);
            break;
        case ModelPreset::CornellBoxSoftShadow:
            triangles = loadObj("cornell-box.obj", 0.17f);
            camera = Camera(glm::vec3{0, 0, 4}, 1400);
            lighting = LightingConfiguration(glm::vec3 {0, 2, 0} * 0.17f, 4.0, 0.1, 16);
            lighting.softShadow = true;
            lighting.lightRadius = 0.1;
            lighting.lightPoints = 100;
            lighting.ambientThreshold = 0.1;
            break;
        case ModelPreset::CornellBoxMirror:
            triangles = loadObj("cornell-box-mirror.obj", 0.17f);
            camera = Camera(glm::vec3{0, 0, 4}, 1400);
            lighting = LightingConfiguration(glm::vec3 {0, 2, 0} * 0.17f, 4.0, 0.1, 16);
            break;
        case ModelPreset::Sphere:
            triangles = loadObj("sphere.obj", 1.0, Colour(255, 0, 0));
            camera = Camera(glm::vec3{0, 2, 6}, 800);
            camera.lookAt(glm::vec3(0, 1.5, 0));
            lighting = LightingConfiguration(glm::vec3 {0, 4, 6}, 200, 0.1, 32);
            lighting.shadingMode = ShadingMode::Phong;
            lighting.proximityWeighting = 50;
            lighting.specularWeighting = 30;
            lighting.angleOfIncidenceWeighting = 20;
            movementSensitivity = 3.0;
            break;
        case ModelPreset::Hackspace:
            triangles = loadObj("hackspace.obj", 1.0);
            camera = Camera(glm::vec3{0, 0, 600}, 600);
            camera.lookAt(glm::vec3{-250, 250, 0});
            renderer = ConcreteRenderer::Rasterised;
            movementSensitivity = 200;
            break;
        default:
            throw std::runtime_error("Invalid Model Preset");
    }
}

void Model::renderFrame(DrawingWindow &window, void(*eventHandler)(SDL_Event event, DrawingWindow &window, Model &m)) {
    SDL_Event event;
    if (window.pollForInputEvents(event) && eventHandler != nullptr) {
        eventHandler(event, window, (Model &) *this);
    };
    window.clearPixels();
    auto r = Renderer::getRenderer(renderer);
    r->draw(window, camera, triangles, lighting);
    window.renderFrame();
}

void keyboardHandler(SDL_Event event, DrawingWindow &window, Model &m) {
    if (event.type == SDL_KEYDOWN) {
        // <-> To position camera
        if (event.key.keysym.sym == SDLK_LEFT) {
            m.camera.position[0] += (0.01f * m.movementSensitivity);
        } else if (event.key.keysym.sym == SDLK_RIGHT) {
            m.camera.position[0] -= (0.01f * m.movementSensitivity);
        } else if (event.key.keysym.sym == SDLK_UP) {
            m.camera.position[1] += (0.01f * m.movementSensitivity);
        } else if (event.key.keysym.sym == SDLK_DOWN) {
            m.camera.position[1] -= (0.01f * m.movementSensitivity);
        // -= To move position backwards / forwards
        } else if (event.key.keysym.sym == SDLK_EQUALS) {
            m.camera.position[2] -= (0.01f * m.movementSensitivity);
        } else if (event.key.keysym.sym == SDLK_MINUS) {
            m.camera.position[2] += (0.01f * m.movementSensitivity);
        // op and kl to rotate position about origin
        } else if (event.key.keysym.sym == SDLK_o) {
            m.camera.rotatePositionAboutX(1);
        } else if (event.key.keysym.sym == SDLK_p) {
            m.camera.rotatePositionAboutX(-1);
        } else if (event.key.keysym.sym == SDLK_k) {
            m.camera.rotatePositionAboutY(1);
        } else if (event.key.keysym.sym == SDLK_l) {
            m.camera.rotatePositionAboutY(-1);
        // WASD to rotate camera orientation
        } else if (event.key.keysym.sym == SDLK_a) {
            m.camera.rotateOrientationAboutY(-1);
        } else if (event.key.keysym.sym == SDLK_d) {
            m.camera.rotateOrientationAboutY(1);
        } else if (event.key.keysym.sym == SDLK_s) {
            m.camera.rotateOrientationAboutX(-1);
        } else if (event.key.keysym.sym == SDLK_w) {
            m.camera.rotateOrientationAboutX(1);
        // Change renderer
        } else if (event.key.keysym.sym == SDLK_1) {
            m.renderer = ConcreteRenderer::Wireframe;
        } else if (event.key.keysym.sym == SDLK_2) {
            m.renderer = ConcreteRenderer::Rasterised;
        } else if (event.key.keysym.sym == SDLK_3) {
            m.renderer = ConcreteRenderer::Raytraced;
        // Change shading mode
        } else if (event.key.keysym.sym == SDLK_4) {
            m.lighting.shadingMode = ShadingMode::None;
        } else if (event.key.keysym.sym == SDLK_5) {
            m.lighting.shadingMode = ShadingMode::Gouraud;
        } else if (event.key.keysym.sym == SDLK_6) {
            m.lighting.shadingMode = ShadingMode::Phong;
        }
        // Change lighting
        else if (event.key.keysym.sym == SDLK_7) {
            m.lighting.proximityLighting = !m.lighting.proximityLighting;
            std::cout << "Proximity Lighting: " << m.lighting.proximityLighting << std::endl;
        } else if (event.key.keysym.sym == SDLK_8) {
            m.lighting.angleOfIncidenceLighting = !m.lighting.angleOfIncidenceLighting;
            std::cout << "Angle-Of-Incidence Lighting: " << m.lighting.angleOfIncidenceLighting << std::endl;
        } else if (event.key.keysym.sym == SDLK_9) {
            m.lighting.specularLighting = !m.lighting.specularLighting;
            std::cout << "Specular Lighting: " << m.lighting.specularLighting << std::endl;
        } else if (event.key.keysym.sym == SDLK_0) {
            m.lighting.ambientLighting = !m.lighting.ambientLighting;
            std::cout << "Ambient Lighting: " << m.lighting.ambientLighting << std::endl;
        }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) window.savePPM("output.ppm");
}

int main(int argc, char *argv[]) {
    try {
        DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
        if (argc >= 2 && strcmp(argv[1], "--animation") == 0) {
            auto driver = AnimationDriver(window);
            driver.start();
        } else {
            auto preset = ModelPreset::CornellBox;
            if (argc >= 2) {
                preset = static_cast<ModelPreset>(std::stoi(argv[1]));
            }
            auto model = Model(preset);
            while (true) {
                model.renderFrame(window, keyboardHandler);
            }
        }
        exit(0);
    }
    catch( const std::exception & ex ) {
        std::cerr << ex.what() << std::endl;
        exit(1);
    }
}
