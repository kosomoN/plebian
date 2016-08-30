#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int cam_width, int cam_height, float cam_fov)
    : width(cam_width), height(cam_height), fov(cam_fov) {
    Resize(width, height);
    UpdateMatrix();
}

void Camera::UpdateMatrix() {
	combined = projectionMatrix * glm::mat4_cast(orientation) * glm::translate(glm::mat4(), -position);
}

void Camera::SetFOV(float cam_fov) {
    fov = cam_fov;
    Resize(width, height);
}

void Camera::Resize(int cam_width, int cam_height) {
    width = cam_width;
    height = cam_height;
    projectionMatrix = glm::perspective(glm::radians(fov), ((float) width) / height, near, far);
}

void Camera::WindowResized(int width, int height)
{
	Resize(width, height);
}
