#include "plebian_server.h"

#include <iostream>
#include <memory>
#include <string>
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <NetworkIDManager.h>
#include <GetTime.h>

#include "network/replica_manager.h"
#include "network/entity_replica.h"
#include "network/network_defines.h"
#include "transform.h"
#include "plebian_game.h"

#define MAX_CLIENTS 16
#define SERVER_PORT 51851

using namespace RakNet;

void PlebianServer::StartServer()
{
    RakPeerInterface *peer = RakPeerInterface::GetInstance();
    Packet *packet;

    PlebianGame game;

    ReplicaManager replica_manager(&game);
    RakNet::NetworkIDManager network_id_manager;
    peer->AttachPlugin(&replica_manager);
    replica_manager.SetNetworkIDManager(&network_id_manager);
    // serialize every tick
    replica_manager.SetAutoSerializeInterval(0);

    entityx::Entity entity = game.entity_manager.create();
    Transform* transform = entity.assign<Transform>().get();
    transform->pos = glm::vec3(1.0f, 2.0f, 3.0f);
    MeshComponent* mesh_component = entity.assign<MeshComponent>().get();
    Mesh mesh;
    mesh.name = "suzanne.obj";
    mesh_component->mesh = &mesh;
    Texture texture;
    texture.name = "suzanne.png";
    mesh_component->texture = &texture;

    Mesh mesh2;
    mesh2.name = "smooth_sphere.obj";
    Texture texture2;
    texture2.name = "yellow.png";

    std::vector<std::shared_ptr<EntityReplica>> balls;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            entityx::Entity ent = game.entity_manager.create();
            Transform* transform2 = ent.assign<Transform>().get();
            transform2->pos = glm::vec3(i * 1.5f  - 6, 2, j * 1.5f - 6);
            mesh_component = ent.assign<MeshComponent>().get();
            mesh_component->mesh = &mesh2;
            mesh_component->texture = &texture2;

            auto replica = std::shared_ptr<EntityReplica>(new EntityReplica(ent, &game));
            replica_manager.Reference(replica.get());
            balls.push_back(replica);
        }
    }



    EntityReplica entity_replica(entity, &game);
    replica_manager.Reference(&entity_replica);

    SocketDescriptor sd(SERVER_PORT, 0);
    peer->Startup(MAX_CLIENTS, &sd, 1);
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);

    printf("Server started.\n");
    bool should_close = false;
    TimeMS start_time = GetTimeMS();
    double game_time, last_update_time = 0, dt, accumulated_ticks = 0;

    game.current_tick = (GetTimeMS() - start_time) / TICK_LENGTH_MS;

    while (!should_close)
    {
        game_time = GetTimeMS() - start_time;
        dt = game_time - last_update_time;
        last_update_time = game_time;

        accumulated_ticks += dt / TICK_LENGTH_MS;
        if (accumulated_ticks >= 1)
        {
            printf("Tick %i\n", game.current_tick);
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

                    clients[packet->guid] = new Client(packet->guid);

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
                    
                    if (clients.count(packet->guid)) {
                        delete clients[packet->guid];
                        clients.erase(packet->guid);
                    }
                    break;
                case ID_CONNECTION_LOST:
                    printf("A client lost the connection.\n");

                    if (clients.count(packet->guid)) {
                        delete clients[packet->guid];
                        clients.erase(packet->guid);
                    }
                    break;
                default:
                    printf("Message with identifier %i has arrived.\n", packet->data[0]);
                    break;
                }

                peer->DeallocatePacket(packet);
            }

            transform->pos = glm::vec3(cos(TICK_LENGTH_MS * game.current_tick / 500.0) * 3, cos(TICK_LENGTH_MS * game.current_tick / 900.0) * 0.5 + cos(TICK_LENGTH_MS * game.current_tick / 600.0) * 0.1, -sin(TICK_LENGTH_MS * game.current_tick / 500.0) * 3);

            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    balls[i * 8 + j]->entity.component<Transform>()->pos.y = cos(game.current_tick * 0.05 + i / 2.f) * 0.8124f + sin(game.current_tick * 0.073498f + j / 2.f);
                }
            }

            game.current_tick++;
            accumulated_ticks--;
        }
    }


    RakPeerInterface::DestroyInstance(peer);
}
