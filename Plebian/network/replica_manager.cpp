#include "replica_manager.h"

#include "transform.h"
#include "entity_replica.h"

RakNet::Replica3* ReplicaConnection::AllocReplica(RakNet::BitStream* allocationIdBitstream, RakNet::ReplicaManager3* replicaManager3)
{
    entityx::Entity entity = entity_manager->create();
    entity.assign<Transform>();
#ifndef SERVER
    entity.assign<TransformHistoryComponent>();
#endif // !SERVER
    return new EntityReplica(entity, tick_time);
}
