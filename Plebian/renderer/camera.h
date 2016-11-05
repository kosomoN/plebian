#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "renderer/window.h"
#include "components.h"

class Camera : public WinResizeListener {
public:
    void InitOrtho(float cam_width, float cam_height, float near, float far);
    void InitPerspective(float cam_width, float cam_height, float fov);

    Transform transform;
	glm::mat4 combined;
    void UpdateMatrix();
    void SetFOV(float fov);
    void Resize(float width, float height);

	virtual void WindowResized(int width, int height);

private:
    bool perspective;
    glm::mat4 projectionMatrix;
    glm::vec3 worldForward = glm::vec3(0, 0, -1);

    float near = 0.01f;
    float far  = 100.0f;

    float width, height;
    float fov;
};

#endif // CAMERA_H_
