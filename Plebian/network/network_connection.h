#ifndef NETWORK_CONNECTION_H_
#define NETOWRK_CONNECTION_H_

#include <string>
#include <RakPeerInterface.h>

struct PacketProcessor {
    virtual void PacketReceived(RakNet::Packet* packet) = 0;
};

class NetworkConnection {
public:
    ~NetworkConnection();
    RakNet::StartupResult Startup();
    RakNet::ConnectionAttemptResult Connect(std::string ip, int port);
    void ProcessPackets();

    RakNet::RakPeerInterface* peer = nullptr;
private:
    PacketProcessor* packet_processor = nullptr;
};

#endif // NETWORK_CONNECTION_H_
