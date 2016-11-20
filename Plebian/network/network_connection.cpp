#include "network_connection.h"

#include "log.h"

using namespace RakNet;

void NetworkConnection::ProcessPackets()
{
    for (Packet* packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
        packet_processor->PacketReceived(packet);
    }
}

ConnectionAttemptResult NetworkConnection::Connect(std::string ip, int port)
{
    Log(Info, "Connecting to %s:%i...", ip.c_str(), port);
    return peer->Connect(ip.c_str(), (unsigned short) port, 0, 0);
}

RakNet::StartupResult NetworkConnection::Startup() {
    if (peer) return RAKNET_ALREADY_STARTED;

    peer = RakPeerInterface::GetInstance();
    SocketDescriptor sd;
    Log(Info, "Starting up RakNet");
    return peer->Startup(1, &sd, 1);
}

NetworkConnection::~NetworkConnection()
{
    if (peer)
        RakPeerInterface::DestroyInstance(peer);
}
