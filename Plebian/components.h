#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <entityx/entityx.h>

#include "transform.h"
#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

struct MeshComponent : entityx::Component<MeshComponent> {
    MeshComponent(Mesh* mesh, Shader* shader, Texture* texture)
        : mesh(mesh), shader(shader), texture(texture) {}
    Mesh* mesh = nullptr;
    Shader* shader = nullptr;
    Texture* texture = nullptr;
};

#endif // COMPONENTS_H_
