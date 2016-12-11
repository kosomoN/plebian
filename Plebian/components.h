#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <entityx/entityx.h>
#include <ReplicaManager3.h>

#include "renderer/mesh.h"
#include "renderer/shader.h"
#include "renderer/texture.h"
#include "plebian_game.h"

extern const size_t ComponentIDToFamily[2];

struct NetworkedComponent {
    virtual uint8_t NetworkID() = 0;
    virtual void Serialize(RakNet::SerializeParameters *serialize_parameters) = 0;
    virtual bool Deserialize(RakNet::DeserializeParameters *deserialize_parameters, PlebianGame* game) = 0;
};

struct MeshComponent : public entityx::Component<MeshComponent>, public NetworkedComponent {
    MeshComponent() {}
    MeshComponent(Mesh* mesh, Material material, Texture* texture)
        : mesh(mesh), material(material), texture(texture) {}
    Mesh* mesh = nullptr;
    Material material;
    Texture* texture = nullptr;

    virtual uint8_t NetworkID();
    virtual void Serialize(RakNet::SerializeParameters *serialize_parameters);
    virtual bool Deserialize(RakNet::DeserializeParameters *deserialize_parameters, PlebianGame* game);
};

#endif // COMPONENTS_H_
