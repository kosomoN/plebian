#include "camera.h"

Camera::Camera(int cam_width, int cam_height, float cam_fov)
    : width(cam_width), height(cam_height), fov(cam_fov) {
    resize(width, height);
    updateMatrix();
}

void Camera::UpdateMatrix() {
    combined = projectionMatrix * viewMatrix;
}

void Camera::SetFOV(float cam_fov) {
    fov = cam_fov;
    resize(width, height);
}

void Camera::Resize(int cam_width, int cam_height) {
    width = cam_width;
    height = cam_height;
    projectionMatrix = glm::perspective(glm::radians(fov), ((float) width) / height, near, far);
}

glm::vec3 Camera::GetWorldPosition() {
    return glm::vec3(viewMatrix[3]);
}
