#ifndef REPLICA_MANAGER_H_
#define REPLICA_MANAGER_H_

#include <RakPeerInterface.h> 
#include <ReplicaManager3.h>
#include <entityx/entityx.h>

class ReplicaConnection : public RakNet::Connection_RM3 {
public:
    ReplicaConnection(const RakNet::SystemAddress& systemAddress_, RakNet::RakNetGUID guid_, entityx::EntityManager* entity_manager, const uint32_t& tick_time)
        : Connection_RM3(systemAddress_, guid_), entity_manager(entity_manager), tick_time(tick_time) {}

    virtual RakNet::Replica3* AllocReplica(RakNet::BitStream *allocationIdBitstream, RakNet::ReplicaManager3 *replicaManager3);

private:
    entityx::EntityManager* entity_manager;
    const uint32_t& tick_time;
};

class ReplicaManager : public RakNet::ReplicaManager3 {
public:
    ReplicaManager(entityx::EntityManager* entity_manager, const uint32_t& tick_time) : entity_manager(entity_manager), tick_time(tick_time) {}

    virtual RakNet::Connection_RM3* AllocConnection(const RakNet::SystemAddress& systemAddress, RakNet::RakNetGUID rakNetGUID) const {
        return new ReplicaConnection(systemAddress, rakNetGUID, entity_manager, tick_time);
    }
    virtual void DeallocConnection(RakNet::Connection_RM3* connection) const {
        delete connection;
    }

private:
    entityx::EntityManager* entity_manager;
    const uint32_t& tick_time;
};

#endif // REPLICA_MANAGER_H_
