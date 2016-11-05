#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include <glm/glm.hpp>
#include "components.h"

struct BaseLight {
    Transform* transform;
    glm::vec3 intensity;
};

struct PointLight : public BaseLight {
    float radius;
};

struct DirectionalLight : public BaseLight {
};

#endif // POINT_LIGHT_H_
