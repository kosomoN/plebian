#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
    Camera(int cam_width, int cam_height, float cam_fov);

    glm::mat4 viewMatrix;

    void UpdateMatrix();
    void SetFOV(float fov);
    void Sesize(int width, int height);
    glm::vec3 GetWorldPosition();
    glm::mat4& GetCombined() { return combined; }

private:
    glm::mat4 projectionMatrix;
    glm::mat4 combined;

    const static float near = 0.1f;
    const static float far  = 100.0f;

    int width, height;
    float fov;
};

#endif // CAMERA_H_
