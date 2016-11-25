#include "server_replica_manager.h"

#include "transform.h"
#include "network/entity_replica.h"

RakNet::Replica3* ReplicaConnection::AllocReplica(RakNet::BitStream* allocationIdBitstream, RakNet::ReplicaManager3* replicaManager3)
{
    entityx::Entity entity = server->entity_manager.create();
    entity.assign<Transform>();
    return new EntityReplica(entity, server);
}
