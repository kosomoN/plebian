#include "replica_manager.h"

#include "transform.h"
#include "entity_replica.h"
#include "renderer/mesh_renderer.h"
#include "log.h"

RakNet::Replica3* ReplicaConnection::AllocReplica(RakNet::BitStream* allocationIdBitstream, RakNet::ReplicaManager3* replicaManager3)
{
    entityx::Entity entity = game->entity_manager.create();
    entity.assign<Transform>();
    entity.assign<TransformHistoryComponent>();
#ifndef SERVER
    entity.assign<MeshComponent>();
    game->mesh_renderer->RegisterEntity(entity);
#endif

    return new EntityReplica(entity, game);
}
