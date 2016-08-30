#ifndef WINDOW_H_
#define WINDOW_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "input/input.h"

struct WinResizeListener {
    virtual void WindowResized(int width, int height) = 0;
};

class Window {
public:
    ~Window();

    std::vector<WinResizeListener*> resizeListeners;

    bool Init(int width, int height);
    void UpdateInput();
    void SwapBuffers();
    void SetInput(Input* in) { input = in; }
    bool ShouldClose() { return glfwWindowShouldClose(glfw_win) != 0; }
    GLFWwindow* GetWindow() { return glfw_win; }
    Input* GetInput() { return input; }

    int width, height;
private:
    GLFWwindow* glfw_win;
    Input* input;
};

#endif // WINDOW_H_
