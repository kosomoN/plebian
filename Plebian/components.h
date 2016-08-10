#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <glm/glm.hpp>
#include <entityx/entityx.h>

#include "renderer/mesh.h"
#include "renderer/shader.h"

struct Transform : entityx::Component<Transform> {
	glm::mat4 transform;
};

struct MeshComponent : entityx::Component<MeshComponent> {
	MeshComponent(Mesh* mesh, Shader* shader) : mesh(mesh), shader(shader) {}
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;
};

#endif // COMPONENTS_H_
