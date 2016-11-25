#ifndef ENTITY_REPLICA_H_
#define ENTITY_REPLICA_H_

#include <ReplicaManager3.h>
#include <entityx/entityx.h>

#ifdef SERVER
#include "plebian_server.h"
#else
#include "plebian_game.h"
#endif

using namespace RakNet;

class EntityReplica : public RakNet::Replica3 {
public:
#ifdef SERVER
    EntityReplica(entityx::Entity entity, PlebianServer* server) : entity(entity), server(server) {}
#else
    EntityReplica(entityx::Entity entity, PlebianGame* game) : entity(entity), game(game) {}
#endif

    virtual void WriteAllocationID(RakNet::Connection_RM3 *destinationConnection, RakNet::BitStream *allocationIdBitstream) const;
    virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3);
    virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection);
    virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {}
    virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) { return true; }
    virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection) {}
    virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection) { return true; }
    virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const;
    virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection);
    virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection);
    virtual RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
private:
    entityx::Entity entity;
#ifdef SERVER
    PlebianServer* server;
#else
    PlebianGame* game;
    uint32_t last_deserialize_tick = 0;
#endif
};

#endif // ENTITY_REPLICA_H_
