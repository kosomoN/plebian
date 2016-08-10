#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <entityx/entityx.h>

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

struct Transform : entityx::Component<Transform> {
    glm::vec3 pos;
    glm::quat orientation;
};

struct MeshComponent : entityx::Component<MeshComponent> {
    MeshComponent(Mesh* mesh, Shader* shader, Texture* texture)
        : mesh(mesh), shader(shader), texture(texture) {}
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
    Texture* texture = nullptr;
};

#endif // COMPONENTS_H_
