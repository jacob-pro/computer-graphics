#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

struct Camera {
    Camera(glm::vec3 position, float focal_length);
    glm::vec3 position;
    glm::mat3 orientation{};
    float focal_length;
    void rotatePositionAboutX(float deg);
    void rotatePositionAboutY(float deg);
    void rotateOrientationAboutX(float deg);
    void rotateOrientationAboutY(float deg);
    void lookAt(glm::vec3 point);
};

#endif //CAMERA_H
