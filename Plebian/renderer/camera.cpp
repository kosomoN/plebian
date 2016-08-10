#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int cam_width, int cam_height, float cam_fov)
    : width(cam_width), height(cam_height), fov(cam_fov) {
    Resize(width, height);
    UpdateMatrix();
}

void Camera::UpdateMatrix() {
    combined = projectionMatrix * viewMatrix;
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

glm::vec3 Camera::GetWorldPosition() {
    return glm::vec3(viewMatrix[3]);
}

void Camera::WindowResized(int width, int height)
{
	Resize(width, height);
}
