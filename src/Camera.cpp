#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h> // NOLINT(modernize-deprecated-headers)

inline double toRadians(double degree) {
    return degree * (M_PI / 180.0);
}

Camera::Camera(glm::vec3 position, float focal_length) : position(position), focal_length(focal_length) {
}

void Camera::rotatePositionAboutX(float deg) {
    auto rad = toRadians(deg);
    auto m = glm::mat3(
            1, 0, 0, // first column (not row!)
            0, cos(rad), sin(rad), // second column
            0, -sin(rad), cos(rad)  // third column
    );
    position = m * position;
}

void Camera::rotatePositionAboutY(float deg) {
    auto rad = toRadians(deg);
    auto m = glm::mat3(
            cos(rad), 0, -sin(rad),
            0, 1, 0,
            sin(rad), 0, cos(rad)
    );
    position = m * position;
}

void Camera::rotateOrientationAboutX(float deg) {
    auto rad = toRadians(deg);
    auto m = glm::mat3(
            1, 0, 0, // first column (not row!)
            0, cos(rad), sin(rad), // second column
            0, -sin(rad), cos(rad)  // third column
    );
    orientation = orientation * m;
}

void Camera::rotateOrientationAboutY(float deg) {
    auto rad = toRadians(deg);
    auto m = glm::mat3(
            cos(rad), 0, -sin(rad),
            0, 1, 0,
            sin(rad), 0, cos(rad)
    );
    orientation = orientation * m;
}

void Camera::lookAt(glm::vec3 point) {
    //Forward = point to camera
    orientation[2] = glm::normalize(position - point);
    // Right = Vertical cross forward
    orientation[0] = glm::normalize(glm::cross(glm::vec3(0,1,0), orientation[2]));
    // Up = Forward cross Right
    orientation[1] = glm::normalize(glm::cross(orientation[2], orientation[0]));
}


