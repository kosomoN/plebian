#ifndef INPUT_H_
#define INPUT_H_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "input/inputlistener.h"

class Input {
public:
    void Init(GLFWwindow* window);
    void HandleKeyEvent(int key, int action);
    void HandleMouseMoveEvent(double xpos, double ypos);
    void HandleMouseEnterEvent(int entered);
    void HandleMouseButtonEvent(int button, int action);
    void AddListener(IInputListener* listener);
    void RemoveListener(IInputListener* listener);
    void ClearListeners();
private:
    std::vector<IInputListener*> listeners;
};

#endif // INPUT_H_
