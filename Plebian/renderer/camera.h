#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "renderer/window.h"
#include "transform.h"

class Camera : public WinResizeListener {
public:
    Camera() {};
    void InitOrtho(float cam_width, float cam_height, float near_plane, float far_plane);
    void InitPerspective(float cam_width, float cam_height, float fov);

    Transform transform;
	glm::mat4 combined;
    void UpdateMatrix();
    void SetFOV(float fov);
    void Resize(float width, float height);

	virtual void WindowResized(int width, int height);

    float GetWidth() { return width; };
    float GetHeight() { return height; };
private:
    bool perspective;
    glm::mat4 projectionMatrix;
    glm::vec3 worldForward = glm::vec3(0, 0, -1);

    float near_plane = 0.01f;
    float far_plane  = 100.0f;

    float width, height;
    float fov;
};

#endif // CAMERA_H_
