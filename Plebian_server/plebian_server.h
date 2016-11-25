#ifndef PLEBIAN_SERVER_H_
#define PLEBIAN_SERVER_H_

#include <vector>
#include <unordered_map>
#include <entityx/entityx.h>

#include "client.h"

struct PlebianServer {
private:
    struct GuidHasher {
        size_t operator()(const RakNet::RakNetGUID& guid) const {
            return guid.g;
        }
    };
public:
    PlebianServer() : entity_manager(events) {}
    void StartServer();

    uint32_t current_tick = 0;

    entityx::EventManager events;
    entityx::EntityManager entity_manager;

    std::unordered_map<RakNet::RakNetGUID, Client*, GuidHasher> clients;
};

#endif // PLEBIAN_SERVER_H_
