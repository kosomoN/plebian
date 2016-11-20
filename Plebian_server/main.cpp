#include <iostream>
#include <string>
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <NetworkIDManager.h>
#include <entityx/entityx.h>
#include <GetTime.h>

#include "network/replica_manager.h"
#include "network/entity_replica.h"
#include "network/network_defines.h"
#include "transform.h"

#define MAX_CLIENTS 16
#define SERVER_PORT 51851

using namespace RakNet;

int main(void)
{
    RakPeerInterface *peer = RakPeerInterface::GetInstance();
    Packet *packet;

    entityx::EventManager events;
    entityx::EntityManager entities(events);

    uint32_t current_tick = 0;

    ReplicaManager replica_manager(&entities, current_tick);
    RakNet::NetworkIDManager network_id_manager;
    peer->AttachPlugin(&replica_manager);
    replica_manager.SetNetworkIDManager(&network_id_manager);
    // serialize every tick
    replica_manager.SetAutoSerializeInterval(0);

    entityx::Entity entity = entities.create();
    Transform* transform = entity.assign<Transform>().get();
    transform->pos = glm::vec3(1.0f, 2.0f, 3.0f);

    EntityReplica entity_replica(entity, current_tick);
    replica_manager.Reference(&entity_replica);

    SocketDescriptor sd(SERVER_PORT, 0);
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
                {
                    printf("A connection is incoming.\n");
                    BitStream bs(sizeof(MessageID) + sizeof(TimeMS));
                    bs.Write((MessageID)ID_START_TIME);
                    bs.Write((TimeMS)start_time);
                    peer->Send(&bs, HIGH_PRIORITY, RELIABLE, 0, packet->guid, false);
                    break;
                }
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

            transform->pos = glm::vec3(cos(TICK_LENGTH_MS * current_tick / 500.0) * 3, cos(TICK_LENGTH_MS * current_tick / 900.0) * 0.5 + cos(TICK_LENGTH_MS * current_tick / 600.0) * 0.1, -sin(TICK_LENGTH_MS * current_tick / 500.0) * 3);

            current_tick++;
            accumulated_ticks--;
        }
    }


    RakPeerInterface::DestroyInstance(peer);
}



