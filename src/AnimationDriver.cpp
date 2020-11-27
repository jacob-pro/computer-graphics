#include "AnimationDriver.h"

AnimationDriver::AnimationDriver(DrawingWindow &window): window(window) {
}

void AnimationDriver::renderAndSaveFrame(Model &model) {
    model.renderFrame(window, nullptr);
    auto fileName = "./animation/" + std::to_string(frameCounter) + ".bmp";
    window.saveBMP(fileName);
    frameCounter++;
}

void AnimationDriver::start() {
    Model model;

    // Show the perspective texturing
    std::cout << "Stage 1: Perspective Corrected Textures Cornell Box" << std::endl;
    model = Model(ModelPreset::CornellBoxTextured);
    auto texturedFrames = 120;
    for (int i = 0; i < texturedFrames; i++) {
        renderAndSaveFrame(model);
        model.camera.rotatePositionAboutX(90.0f / (float) texturedFrames);
        model.camera.lookAt(glm::vec3{0,0,0});
    }

    // Show the hackspace textured
    std::cout << "Stage 2: Perspective Corrected Textures Hackspace Logo" << std::endl;
    model = Model(ModelPreset::Hackspace);
    auto hackspaceFrames = 120;
    for (int i = 0; i < hackspaceFrames; i++) {
        renderAndSaveFrame(model);
        model.camera.rotatePositionAboutY(180.0f / (float) hackspaceFrames);
        model.camera.lookAt(glm::vec3{-250, 250, 0});
    }

    // Show the soft shadow
    std::cout << "Stage 3: Soft Shadows in the Cornell Box" << std::endl;
    model = Model(ModelPreset::CornellBoxSoftShadow);
    auto initialPos = model.camera.position;
    auto softShadowFrames = 150;
    for (int i = 0; i < softShadowFrames; i++) {
        auto progress = int((float) i / (float) softShadowFrames * 100.0f);
        std::cout << "\r" << progress << "%" << std::flush;
        model.camera.position = initialPos;
        auto angle = -30.0f + (60.0f / (float) softShadowFrames * (float) i);
        model.camera.rotatePositionAboutY(angle);
        model.camera.lookAt(glm::vec3{0,0,0});
        renderAndSaveFrame(model);
    }

    // Show the lighting
    std::cout << "Stage 4: Sphere Shading" << std::endl;
    model = Model(ModelPreset::Sphere);
    model.lighting.shadingMode = ShadingMode::None;
    auto sphereFrames = 120;
    for (int i = 0; i < sphereFrames; i++) {
        auto progress = int((float) i / (float) sphereFrames * 100.0f);
        std::cout << "\r" << progress << "%" << std::flush;
        if (i == round(sphereFrames / 3)) model.lighting.shadingMode = ShadingMode::Gouraud;
        if (i == round(2.0 *sphereFrames / 3)) model.lighting.shadingMode = ShadingMode::Phong;
        renderAndSaveFrame(model);
        model.camera.rotatePositionAboutY(0.05);
        model.camera.rotatePositionAboutX(-0.05);
        model.camera.lookAt(glm::vec3(0, 1.5, 0));
    }

    // Show the mirror
    std::cout << "Stage 5: Cornell Box With Mirror" << std::endl;
    model = Model(ModelPreset::CornellBoxMirror);
    auto initialPos2 = model.camera.position;
    auto mirrorFrames = 150;
    for (int i = 0; i < mirrorFrames; i++) {
        model.camera.position = initialPos2;
        auto angle = 10 - (50.0f / (float) mirrorFrames * (float) i);
        model.camera.rotatePositionAboutY(angle);
        model.camera.lookAt(glm::vec3{0,0,0});
        renderAndSaveFrame(model);
    }
}

