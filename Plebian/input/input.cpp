#include "input.h"

#include <algorithm>
#include "log.h"
#include "renderer/window.h"

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleKeyEvent(key, action);
}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleMouseMoveEvent(xpos, ypos);
}

void glfw_cursor_enter_callback(GLFWwindow* window, int entered) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleMouseEnterEvent(entered);
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleMouseButtonEvent(button, action);
}

void Input::Init(GLFWwindow* window) {
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
    glfwSetCursorEnterCallback(window, glfw_cursor_enter_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
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

void Input::HandleMouseMoveEvent(double xpos, double ypos) {
    for (IInputListener* listener : listeners) {
        if (listener->MouseMoved(xpos, ypos))
            break;
    }
}

void Input::HandleMouseEnterEvent(int entered) {
    if (entered) {
        for (IInputListener* listener : listeners) {
            if (listener->MouseEnter())
                break;
        }
    } else {
        for (IInputListener* listener : listeners) {
            if (listener->MouseLeave())
                break;
        }
    }
}

void Input::HandleMouseButtonEvent(int button, int action) {
    if (action == GLFW_PRESS) {
        for (IInputListener* listener : listeners) {
            if (listener->MouseDown(button))
                break;
        }
    } else {
        for (IInputListener* listener : listeners) {
            if (listener->MouseUp(button))
                break;
        }
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
