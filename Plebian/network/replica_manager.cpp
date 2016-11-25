#include "replica_manager.h"

#include "transform.h"
#include "entity_replica.h"

RakNet::Replica3* ReplicaConnection::AllocReplica(RakNet::BitStream* allocationIdBitstream, RakNet::ReplicaManager3* replicaManager3)
{
    entityx::Entity entity = plebian_game->entity_manager.create();
    entity.assign<Transform>();
    entity.assign<TransformHistoryComponent>();

    return new EntityReplica(entity, plebian_game);
}
