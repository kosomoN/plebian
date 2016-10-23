#include "input.h"

#include <algorithm>
#include "log.h"
#include "renderer/window.h"

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleKeyEvent(window, key, action);
}

void glfw_char_callback(GLFWwindow* window, unsigned int codepoint) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleCharEvent(window, codepoint);
}

void glfw_charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleCharModsEvent(window, codepoint, mods);
}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleMouseMoveEvent(window, xpos, ypos);
}

void glfw_cursor_enter_callback(GLFWwindow* window, int entered) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleMouseEnterEvent(window, entered);
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleMouseButtonEvent(window, button, action);
}

void glfw_mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Window* win = (Window*) glfwGetWindowUserPointer(window);
    win->GetInput()->HandleMouseScrollEvent(window, xoffset, yoffset);
}

void Input::Init(GLFWwindow* window) {
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetCharCallback(window, glfw_char_callback);
    glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
    glfwSetCursorEnterCallback(window, glfw_cursor_enter_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    glfwSetScrollCallback(window, glfw_mouse_scroll_callback);
}

void Input::HandleKeyEvent(GLFWwindow* window, int key, int action) {
    if (action == GLFW_PRESS) {
        for (IInputListener* listener : listeners) {
            if (listener->KeyDown(window, key))
                break;
        }
    } else if (action == GLFW_RELEASE) {
        for (IInputListener* listener : listeners) {
            if (listener->KeyUp(window, key))
                break;
        }
    } else if (action == GLFW_REPEAT) {
        for (IInputListener* listener : listeners) {
            if (listener->KeyRepeat(window, key))
                break;
        }
    } else {
        Log(Warn, "Detected unknown GLFW key action");
    }
}

void Input::HandleCharEvent(GLFWwindow* window, unsigned int codepoint) {
    for (IInputListener* listener : listeners) {
        if (listener->CharTyped(window, codepoint))
            break;
    }
}

void Input::HandleCharModsEvent(GLFWwindow* window, unsigned int codepoint, int mods) {
    for (IInputListener* listener : listeners) {
        if (listener->CharModsTyped(window, codepoint, mods))
            break;
    }
}

void Input::HandleMouseMoveEvent(GLFWwindow* window, double xpos, double ypos) {
    for (IInputListener* listener : listeners) {
        if (listener->MouseMoved(window, xpos, ypos))
            break;
    }
}

void Input::HandleMouseEnterEvent(GLFWwindow* window, int entered) {
    if (entered) {
        for (IInputListener* listener : listeners) {
            if (listener->MouseEnter(window))
                break;
        }
    } else {
        for (IInputListener* listener : listeners) {
            if (listener->MouseLeave(window))
                break;
        }
    }
}

void Input::HandleMouseButtonEvent(GLFWwindow* window, int button, int action) {
    if (action == GLFW_PRESS) {
        for (IInputListener* listener : listeners) {
            if (listener->MouseDown(window, button))
                break;
        }
    } else {
        for (IInputListener* listener : listeners) {
            if (listener->MouseUp(window, button))
                break;
        }
    }
}

void Input::HandleMouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset) {
    for (IInputListener* listener : listeners) {
        if (listener->MouseScrolled(window, xoffset, yoffset))
            break;
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
