#ifndef FP_CAMERA_CONTROLLER_H_
#define FP_CAMERA_CONTROLLER_H_

#include <glm/glm.hpp>
#include "input/inputlistener.h"
#include "renderer/camera.h"

class FPCameraController : public IInputListener
{
public:
    FPCameraController(Camera* cam) :
        camera(cam),
        viewDir(0.0f, 0.0f, -1.0f),
        speed(5.f),
        oldX(0),
        oldY(0) {}

    virtual bool KeyUp(GLFWwindow* window, int key);
    virtual bool KeyDown(GLFWwindow* window, int key);
    virtual bool MouseMoved(GLFWwindow* window, double xpos, double ypos);
    virtual bool MouseUp(GLFWwindow* window, int button) { return true; }
    virtual bool MouseDown(GLFWwindow* window, int button) { return true; }

    void Update(float delta);

private:
    Camera* camera;
    float speed;
    bool forward, backwards, left, right, up, down;
    double oldX, oldY;
    float yaw, pitch;
    float sensitivity = 0.001f;
    int oldCursorMode;
    glm::vec3 viewDir;
};

#endif // FP_CAMERA_CONTROLLER_H_