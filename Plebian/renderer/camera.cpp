#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::InitOrtho(float cam_width, float cam_height, float near_, float far_)
{
    perspective = false;
    near = near_;
    far = far_;
    Resize(cam_width, cam_height);
    UpdateMatrix();
}

void Camera::InitPerspective(float cam_width, float cam_height, float cam_fov)
{
    perspective = true;
    fov = cam_fov;
    Resize(cam_width, cam_height);
    UpdateMatrix();
}

void Camera::UpdateMatrix() {
	combined = projectionMatrix * glm::mat4_cast(orientation) * glm::translate(glm::mat4(), -position);
}

void Camera::SetFOV(float cam_fov) {
    fov = cam_fov;
    Resize(width, height);
}

void Camera::Resize(float cam_width, float cam_height) {
    width = cam_width;
    height = cam_height;
    if (perspective)
        projectionMatrix = glm::perspective(glm::radians(fov), width / height, near, far);
    else
        projectionMatrix = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, near, far);
}

void Camera::WindowResized(int width, int height)
{
	Resize((float) width, (float) height);
}
