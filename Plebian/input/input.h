#ifndef INPUT_H_
#define INPUT_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "input/inputlistener.h"

class Input {
public:
    void Init(GLFWwindow* window);
    void HandleKeyEvent(GLFWwindow* window, int key, int action);
    void HandleCharEvent(GLFWwindow* window, unsigned int codepoint);
    void HandleCharModsEvent(GLFWwindow* window, unsigned int codepoint, int mods);
    void HandleMouseMoveEvent(GLFWwindow* window, double xpos, double ypos);
    void HandleMouseEnterEvent(GLFWwindow* window, int entered);
    void HandleMouseButtonEvent(GLFWwindow* window, int button, int action);
    void HandleMouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset);
    void AddListener(IInputListener* listener);
    void RemoveListener(IInputListener* listener);
    void ClearListeners();
private:
    std::vector<IInputListener*> listeners;
};

#endif // INPUT_H_
