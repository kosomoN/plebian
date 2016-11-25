#ifndef CLIENT_H_
#define CLIENT_H_

#include <RakPeerInterface.h>

class EntityReplica;

struct Client {
    Client(RakNet::RakNetGUID& guid) : guid(guid) {}

    RakNet::RakNetGUID& guid;
    EntityReplica* player_entity;
};

#endif // CLIENT_H_
