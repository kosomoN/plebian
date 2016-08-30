#include "window.h"
#include "log.h"

Window::~Window() {
    glfwTerminate();
}

void glfw_error_callback(int error, const char* description) {
    Log(Error, "GLFW Error: %s", description);
}

void framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height) {
    glViewport(0, 0, width, height);
    Window* win = (Window*) glfwGetWindowUserPointer(glfw_window);
    win->width = width;
    win->height = height;
    for (WinResizeListener* listener : win->resizeListeners)
        listener->WindowResized(width, height);
}

bool Window::Init(int width_, int height_) {
    width = width_;
    height = height_;
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        Log(Error, "Failed to initialize GLFW.");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfw_win = glfwCreateWindow(width, height, "plebian", NULL, NULL);
    if (!glfw_win) {
        Log(Error, "Failed to create GLFW window.");
        glfwTerminate();
        return false;
    }
    glfwSetWindowUserPointer(glfw_win, this);

    glfwMakeContextCurrent(glfw_win);
    glfwSetFramebufferSizeCallback(glfw_win, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        Log(Error, "GLEW Error: %s", glewGetErrorString(err));
        return false;
    }
    Log(Info, "Using GLEW %s", glewGetString(GLEW_VERSION));

    return true;
}

void Window::UpdateInput() {
    glfwPollEvents();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(glfw_win);
}
