#ifndef CG2020_ANIMATIONDRIVER_H
#define CG2020_ANIMATIONDRIVER_H


#include <DrawingWindow.h>
#include "Camera.h"
#include "LightingConfiguration.h"
#include "main.h"

// A class to manage creating the frames for the video
// 10s at 60fps = need to save 600 frames
class AnimationDriver {
public:
    explicit AnimationDriver(DrawingWindow &window);
    void start();
private:
    uint32_t frameCounter{};
    DrawingWindow &window;
    void renderAndSaveFrame(Model &model);
};



#endif //CG2020_ANIMATIONDRIVER_H
