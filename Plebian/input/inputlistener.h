#ifndef IINPUTLISTENER_H_
#define IINPUTLISTENER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct IInputListener {
    virtual bool KeyUp(GLFWwindow* window, int key) { return false; }
    virtual bool KeyDown(GLFWwindow* window, int key) { return false; }
    virtual bool KeyRepeat(GLFWwindow* window, int key) { return false; }
    virtual bool CharTyped(GLFWwindow* window, unsigned int codepoint) { return false; }
    virtual bool CharModsTyped(GLFWwindow* window, unsigned int codepoint, int mods) { return false; }
    virtual bool MouseMoved(GLFWwindow* window, double xpos, double ypos) { return false; }
    virtual bool MouseUp(GLFWwindow* window, int button) { return false; }
    virtual bool MouseDown(GLFWwindow* window, int button) { return false; }
    virtual bool MouseEnter(GLFWwindow* window) { return false; }
    virtual bool MouseLeave(GLFWwindow* window) { return false; }
    virtual bool MouseScrolled(GLFWwindow* window, double xoffset, double yoffset) { return false; }
};

#endif //IINPUTLISTENER_H_
