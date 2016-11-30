#ifndef PLEBIAN_SERVER_H_
#define PLEBIAN_SERVER_H_

#include <vector>
#include <unordered_map>
#include <entityx/entityx.h>
#include <cstddef>

#include "client.h"

struct PlebianServer {
private:
    struct GuidHasher {
        size_t operator()(const RakNet::RakNetGUID& guid) const {
            return guid.g;
        }
    };
public:
    void StartServer();

    std::unordered_map<RakNet::RakNetGUID, Client*, GuidHasher> clients;
};

#endif // PLEBIAN_SERVER_H_
