#include <iostream>
#include <string>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"

#define MAX_CLIENTS 16
#define SERVER_PORT 51851

int main(void)
{
    RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
    RakNet::Packet *packet;

    RakNet::SocketDescriptor sd(SERVER_PORT, 0);
    peer->Startup(MAX_CLIENTS, &sd, 1);
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);

    printf("Server started.\n");
    while (1)
    {
        for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
        {
            switch (packet->data[0])
            {
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                printf("Another client has disconnected.\n");
                break;
            case ID_REMOTE_CONNECTION_LOST:
                printf("Another client has lost the connection.\n");
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                printf("Another client has connected.\n");
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                printf("Our connection request has been accepted.\n");
                break;
            case ID_NEW_INCOMING_CONNECTION:
                printf("A connection is incoming.\n");
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                printf("The server is full.\n");
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                printf("A client has disconnected.\n");
                break;
            case ID_CONNECTION_LOST:
                printf("A client lost the connection.\n");
                break;
            default:
                printf("Message with identifier %i has arrived.\n", packet->data[0]);
                break;
            }
        }
    }


    RakNet::RakPeerInterface::DestroyInstance(peer);
}



