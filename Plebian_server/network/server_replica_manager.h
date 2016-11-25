#ifndef SERVER_REPLICA_MANAGER_H_
#define SERVER_REPLICA_MANAGER_H_

#include <RakPeerInterface.h> 
#include <ReplicaManager3.h>
#include <entityx/entityx.h>

#include "plebian_server.h"

class ReplicaConnection : public RakNet::Connection_RM3 {
public:
    ReplicaConnection(const RakNet::SystemAddress& systemAddress_, RakNet::RakNetGUID guid_, PlebianServer* server)
        : Connection_RM3(systemAddress_, guid_), server(server) {}

    virtual RakNet::Replica3* AllocReplica(RakNet::BitStream *allocationIdBitstream, RakNet::ReplicaManager3 *replicaManager3);

private:
    PlebianServer* server = nullptr;
};

class ReplicaManager : public RakNet::ReplicaManager3 {
public:
    ReplicaManager(PlebianServer* server) : server(server) {}
    virtual RakNet::Connection_RM3* AllocConnection(const RakNet::SystemAddress& systemAddress, RakNet::RakNetGUID rakNetGUID) const {
        return new ReplicaConnection(systemAddress, rakNetGUID, server);
    }
    virtual void DeallocConnection(RakNet::Connection_RM3* connection) const {
        delete connection;
    }

private:
    PlebianServer* server = nullptr;
};

#endif // SERVER_REPLICA_MANAGER_H_
