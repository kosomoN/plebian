#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <glm/glm.hpp>
#include <entityx/entityx.h>

#include "renderer/mesh.h"

struct Transform : entityx::Component<Transform> {
	glm::mat4 transform;
};

struct MeshComponent : entityx::Component<MeshComponent> {
	Mesh* mesh = nullptr;
};

#endif // COMPONENTS_H_
