#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <entityx/entityx.h>
#include <ReplicaManager3.h>

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "plebian_game.h"

extern const size_t ComponentIDToFamily[1];

struct NetworkedComponent {
    virtual uint8_t NetworkID() = 0;
    virtual void Serialize(RakNet::SerializeParameters *serialize_parameters) = 0;
    virtual bool Deserialize(RakNet::DeserializeParameters *deserialize_parameters, PlebianGame* game) = 0;
};

#ifndef SERVER
struct MeshComponent : entityx::Component<MeshComponent> {
    MeshComponent() {}
    MeshComponent(Mesh* mesh, Material material, Shader* shader, Texture* texture)
        : mesh(mesh), material(material), shader(shader), texture(texture) {}
    Mesh* mesh = nullptr;
    Material material;
    Shader* shader = nullptr;
    Texture* texture = nullptr;
};
#endif // !SERVER

#endif // COMPONENTS_H_
