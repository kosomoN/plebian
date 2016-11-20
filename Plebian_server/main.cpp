#include <iostream>
#include <string>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <NetworkIDManager.h>
#include <entityx/entityx.h>
#include <GetTime.h>

#define MAX_CLIENTS 16
#define SERVER_PORT 51851

#define TICK_LENGTH_MS (1000.0f / 20.0f)

using namespace RakNet;

int main(void)
{
    RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
    RakNet::Packet *packet;

    entityx::EventManager events;
    entityx::EntityManager entities(events);

    uint32_t current_tick = 0;

    RakNet::SocketDescriptor sd(SERVER_PORT, 0);
    peer->Startup(MAX_CLIENTS, &sd, 1);
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);

    printf("Server started.\n");
    bool should_close = false;
    TimeMS start_time = GetTimeMS();
    double game_time, last_update_time = 0, dt, accumulated_ticks = 0;

    current_tick = (GetTimeMS() - start_time) / TICK_LENGTH_MS;

    while (!should_close)
    {
        game_time = GetTimeMS() - start_time;
        dt = game_time - last_update_time;
        last_update_time = game_time;

        accumulated_ticks += dt / TICK_LENGTH_MS;
        if (accumulated_ticks >= 1)
        {
            while (packet = peer->Receive())
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

                peer->DeallocatePacket(packet);
            }

            current_tick++;
            accumulated_ticks--;
        }
    }


    RakNet::RakPeerInterface::DestroyInstance(peer);
}



