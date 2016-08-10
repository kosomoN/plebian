#include "input.h"

#include <algorithm>
#include "log.h"
#include "renderer/window.h"

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleKeyEvent(key, action);
}

void Input::Init(GLFWwindow* window) {
    glfwSetKeyCallback(window, glfw_key_callback);
}

void Input::HandleKeyEvent(int key, int action) {
    if (action == GLFW_PRESS) {
        for (IInputListener* listener : listeners) {
            if (listener->KeyDown(key))
                break;
        }
    } else if (action == GLFW_RELEASE) {
        for (IInputListener* listener : listeners) {
            if (listener->KeyUp(key))
                break;
        }
    } else if (action == GLFW_REPEAT) {
        for (IInputListener* listener : listeners) {
            if (listener->KeyRepeat(key))
                break;
        }
    } else {
        Log(Warn, "Detected unknown GLFW key action");
    }
}

void Input::AddListener(IInputListener* listener) {
    listeners.push_back(listener);
}

void Input::RemoveListener(IInputListener* listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void Input::ClearListeners() {
   listeners.clear();
}
