#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "renderer/window.h"

class Camera : public WinResizeListener {
public:
    Camera(int cam_width, int cam_height, float cam_fov);

	glm::quat orientation;
	glm::vec3 position;
	glm::mat4 combined;
    void UpdateMatrix();
    void SetFOV(float fov);
    void Resize(int width, int height);

	virtual void WindowResized(int width, int height);

private:
    glm::mat4 projectionMatrix;

    constexpr static float near = 0.01f;
    constexpr static float far  = 100.0f;

    int width, height;
    float fov;
};

#endif // CAMERA_H_
