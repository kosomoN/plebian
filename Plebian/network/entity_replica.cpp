#include "entity_replica.h"

#include <GetTime.h>

#include "components.h"
#include "transform.h"
#include "log.h"
#include "renderer/mesh_renderer.h"

void EntityReplica::WriteAllocationID(Connection_RM3 *destinationConnection, BitStream *allocationIdBitstream) const
{
    allocationIdBitstream->Write((uint8_t) 0);
}

RM3ConstructionState EntityReplica::QueryConstruction(Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3)
{
#ifdef SERVER
    return QueryConstruction_ServerConstruction(destinationConnection, true);
#else
    return QueryConstruction_ServerConstruction(destinationConnection, false);
#endif
}

bool EntityReplica::QueryRemoteConstruction(Connection_RM3 *sourceConnection)
{
#ifdef SERVER
    return QueryRemoteConstruction_ServerConstruction(sourceConnection, true);
#else
    return QueryRemoteConstruction_ServerConstruction(sourceConnection, false);
#endif
}

RM3ActionOnPopConnection EntityReplica::QueryActionOnPopConnection(Connection_RM3 *droppedConnection) const
{
#ifdef SERVER
    return QueryActionOnPopConnection_Server(droppedConnection);
#else
    return QueryActionOnPopConnection_Client(droppedConnection);
#endif
}

RM3QuerySerializationResult EntityReplica::QuerySerialization(Connection_RM3 *destinationConnection)
{
#ifdef SERVER
    return QuerySerialization_ServerSerializable(destinationConnection, true);
#else
    return QuerySerialization_ServerSerializable(destinationConnection, false);
#endif
}

RM3SerializationResult EntityReplica::Serialize(SerializeParameters *ser)
{
    ser->messageTimestamp = game->current_tick;

    //TODO clean up
    entity.component<Transform>()->Serialize(ser);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void EntityReplica::Deserialize(RakNet::DeserializeParameters *deser)
{
    uint32_t packet_tick = deser->timeStamp;

    if (packet_tick != 0 && game->last_received_snapshot < packet_tick) {
        game->NewSnapshot(packet_tick);
    }
    
    for (int i = 0; i < RM3_NUM_OUTPUT_BITSTREAM_CHANNELS; i++) {
        if (!deser->bitstreamWrittenTo[i])
            continue;

        uint8_t component_id;
        while (deser->serializationBitstream[i].Read<uint8_t>(component_id)) {
            if (component_id >= sizeof(ComponentIDToFamily) / sizeof(ComponentIDToFamily[0]))
                break;

            size_t component_family = ComponentIDToFamily[component_id];
            NetworkedComponent* net_component = static_cast<NetworkedComponent*>(entity.component(component_family));

            // if the component has a transform history component, let it deserialize
            if (component_family == Transform::Family()) {
                if (entity.has_component<TransformHistoryComponent>()) {
                    if (!entity.component<TransformHistoryComponent>()->DeserializeState(deser))
                        break;
                    continue;
                }
            }

            // if a component fails to deserialize, stop because the packet is probably malformed
            if (net_component == nullptr || !net_component->Deserialize(deser, game))
                break;
        }
    }
}

void EntityReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection)
{
#ifndef SERVER
    game->mesh_renderer->UnregisterEntity(entity);
#endif
    entity.destroy();
    delete this;
}
