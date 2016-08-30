#include "fp_camera_controller.h"

#include <GLFW/glfw3.h>
#include "log.h"

void FPCameraController::Update(float delta) {
    glm::vec3 rightDir = glm::cross(viewDir, glm::vec3(0, 1, 0));
    if (forward)   camera->position += viewDir * speed * delta;
    if (backwards) camera->position -= viewDir * speed * delta;
    if (right)     camera->position += rightDir * speed * delta;
    if (left)      camera->position -= rightDir * speed * delta;
    if (up)        camera->position.y += speed * delta;
    if (down)      camera->position.y -= speed * delta;
}

bool FPCameraController::KeyUp(int key) {
    switch (key) {
    case GLFW_KEY_W:
        forward = false;
        break;
    case GLFW_KEY_S:
        backwards = false;
        break;
    case GLFW_KEY_A:
        left = false;
        break;
    case GLFW_KEY_D:
        right = false;
        break;
    case GLFW_KEY_SPACE:
        up = false;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        down = false;
        break;
    }
    return false;
}

bool FPCameraController::KeyDown(int key) {
    switch (key) {
    case GLFW_KEY_W:
        forward = true;
        break;
    case GLFW_KEY_S:
        backwards = true;
        break;
    case GLFW_KEY_A:
        left = true;
        break;
    case GLFW_KEY_D:
        right = true;
        break;
    case GLFW_KEY_SPACE:
        up = true;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        down = true;
        break;
    }
    return false;
}