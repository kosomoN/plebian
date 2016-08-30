#ifndef FP_CAMERA_CONTROLLER_H_
#define FP_CAMERA_CONTROLLER_H_

#include <glm/glm.hpp>
#include "input/inputlistener.h"
#include "renderer/camera.h"

class FPCameraController : public IInputListener
{
public:
    FPCameraController(Camera* cam) : camera(cam),
        viewDir(0.0f, 0.0f, -1.0f),
        speed(5.f) {}

    virtual bool KeyUp(int key);
    virtual bool KeyDown(int key);

    void Update(float delta);

private:
    Camera* camera;
    float speed;
    bool forward, backwards, left, right, up, down;
    glm::vec3 viewDir;
};

#endif // FP_CAMERA_CONTROLLER_H_