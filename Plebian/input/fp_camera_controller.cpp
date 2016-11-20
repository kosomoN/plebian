#define NOMINMAX
#include "fp_camera_controller.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include "log.h"

void FPCameraController::Update(float delta) {
    glm::vec3 right_dir = glm::normalize(glm::cross(view_dir, glm::vec3(0, 1, 0)));
    if (forward)   camera->transform.pos += view_dir * speed * delta;
    if (backwards) camera->transform.pos -= view_dir * speed * delta;
    if (right)     camera->transform.pos += right_dir * speed * delta;
    if (left)      camera->transform.pos -= right_dir * speed * delta;
    if (up)        camera->transform.pos.y += speed * delta;
    if (down)      camera->transform.pos.y -= speed * delta;
}

bool FPCameraController::KeyUp(GLFWwindow* window, int key) {
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

bool FPCameraController::KeyDown(GLFWwindow* window, int key) {
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

bool FPCameraController::MouseMoved(GLFWwindow* window, double xpos, double ypos) {
	int mode = glfwGetInputMode(window, GLFW_CURSOR);
    if (mode != GLFW_CURSOR_DISABLED) {
        oldCursorMode = mode;
        return false;
    }

	if (oldCursorMode == GLFW_CURSOR_DISABLED) {
		pitch += (float)(xpos - oldX) * sensitivity;
		yaw += (float)(ypos - oldY) * sensitivity;
		yaw = glm::min(glm::max(yaw, -0.49f * glm::pi<float>()), 0.49f * glm::pi<float>());
		camera->transform.orientation = glm::angleAxis(yaw, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(pitch, glm::vec3(0.0f, 1.0f, 0.0f));
        view_dir = glm::vec3(0.0f, 0.0f, -1.0f) * camera->transform.orientation;
	}

	oldX = xpos;
	oldY = ypos;
	oldCursorMode = mode;

    return true;
}
