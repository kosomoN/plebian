#include "components.h"

#include "transform.h"
#include "io/meshloader.h"
#include "io/textureloader.h"
#include "renderer/mesh_renderer.h"

const size_t ComponentIDToFamily[] = {
    Transform::family(),
    MeshComponent::family()
};

uint8_t Transform::NetworkID() { return 0; }
uint8_t MeshComponent::NetworkID() { return 1; }

void MeshComponent::Serialize(RakNet::SerializeParameters *ser)
{
#ifdef SERVER
    ser->outputBitstream[1].Write<uint8_t>(NetworkID());
    ser->outputBitstream[1].Write(RakNet::RakString(mesh->name.c_str()));
    ser->outputBitstream[1].Write(RakNet::RakString(texture->name.c_str()));
#endif
}

bool MeshComponent::Deserialize(RakNet::DeserializeParameters *deser, PlebianGame* game)
{
    RakNet::RakString mesh_name, texture_name;
    if (!deser->serializationBitstream[1].Read(mesh_name)) return false;
    if (!deser->serializationBitstream[1].Read(texture_name)) return false;
#ifndef SERVER
    mesh = game->mesh_loader->GetMesh(std::string(mesh_name.C_String()));
    texture = game->texture_loader->GetTexture2d(std::string(texture_name.C_String()));
#endif

    return true;
}
