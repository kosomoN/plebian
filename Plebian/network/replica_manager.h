#ifndef REPLICA_MANAGER_H_
#define REPLICA_MANAGER_H_

#include <RakPeerInterface.h> 
#include <ReplicaManager3.h>
#include <entityx/entityx.h>

#include "plebian_game.h"

class ReplicaConnection : public RakNet::Connection_RM3 {
public:
    ReplicaConnection(const RakNet::SystemAddress& systemAddress_, RakNet::RakNetGUID guid_, PlebianGame* game)
        : Connection_RM3(systemAddress_, guid_), game(game) {}

    virtual RakNet::Replica3* AllocReplica(RakNet::BitStream *allocationIdBitstream, RakNet::ReplicaManager3 *replicaManager3);

private:
    PlebianGame* game;
};

class ReplicaManager : public RakNet::ReplicaManager3 {
public:
    ReplicaManager(PlebianGame* game) : game(game) {}

    virtual RakNet::Connection_RM3* AllocConnection(const RakNet::SystemAddress& systemAddress, RakNet::RakNetGUID rakNetGUID) const {
        return new ReplicaConnection(systemAddress, rakNetGUID, game);
    }
    virtual void DeallocConnection(RakNet::Connection_RM3* connection) const {
        delete connection;
    }

private:
    PlebianGame* game;
};

#endif // REPLICA_MANAGER_H_
