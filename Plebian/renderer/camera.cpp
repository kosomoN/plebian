#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::InitOrtho(float cam_width, float cam_height, float near_plane_, float far_plane_)
{
    perspective = false;
    near_plane = near_plane_;
    far_plane = far_plane_;
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
	combined = projectionMatrix * glm::mat4_cast(transform.orientation) * glm::translate(glm::mat4(), -transform.pos);
}

void Camera::SetFOV(float cam_fov) {
    fov = cam_fov;
    Resize(width, height);
}

void Camera::Resize(float cam_width, float cam_height) {
    width = cam_width;
    height = cam_height;
    if (perspective)
        projectionMatrix = glm::perspective(glm::radians(fov), width / height, near_plane, far_plane);
    else
        projectionMatrix = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, near_plane, far_plane);
}

void Camera::WindowResized(int width, int height)
{
	Resize((float) width, (float) height);
}
