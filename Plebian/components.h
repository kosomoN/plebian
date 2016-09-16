#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entityx/entityx.h>

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

struct Transform : entityx::Component<Transform> {
    glm::vec3 pos;
    Transform* parent = nullptr;
    glm::quat orientation;

    glm::mat4 WorldSpace() {
        glm::mat4 worldSpace;
        ParentWorldSpace(worldSpace);
        return worldSpace;
    }

private:

    void ParentWorldSpace(glm::mat4& child) {
        child = glm::mat4_cast(orientation) * child;
        child = glm::translate(glm::mat4(), pos) * child;
        if (parent)
            parent->ParentWorldSpace(child);
    }
};

struct MeshComponent : entityx::Component<MeshComponent> {
    MeshComponent(Mesh* mesh, Shader* shader, Texture* texture)
        : mesh(mesh), shader(shader), texture(texture) {}
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
    Texture* texture = nullptr;
};

#endif // COMPONENTS_H_
