#include "entity_replica.h"

#include <GetTime.h>

#include "components.h"
#include "transform.h"
#include "log.h"

using namespace entityx;

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
    return QueryActionOnPopConnection_Server(droppedConnection);
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
    ser->outputBitstream[0].Write(tick_time);

    ComponentHandle<Transform> t = entity.component<Transform>();
    ser->outputBitstream[0].Write<uint8_t>(static_cast<NetworkedComponent*>(t.get())->NetworkID());
    t->Serialize(ser);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void EntityReplica::Deserialize(RakNet::DeserializeParameters *deser)
{
    entityx::uint32_t packet_tick;
    deser->serializationBitstream[0].Read(packet_tick);
    
    uint8_t component_id;
    while (deser->serializationBitstream[0].Read<uint8_t>(component_id)) {
        size_t component_family = ComponentIDToFamily[component_id];
        NetworkedComponent* net_component = static_cast<NetworkedComponent*>(entity.component(component_family));

        // if the component has a transform history component, let it deserialize
        if (component_family == Transform::Family()) {
            if (entity.has_component<TransformHistoryComponent>()) {
                if (!entity.component<TransformHistoryComponent>()->DeserializeState(deser, packet_tick))
                    break;
                continue;
            }
        }

        Log(Info, "Component ID %i", component_id);

        // if a component fails to deserialize, stop because the packet is probably malformed
        if (!net_component->Deserialize(deser))
            break;
    }
}

void EntityReplica::DeallocReplica(RakNet::Connection_RM3 *sourceConnection)
{
    entity.destroy();
    delete this;
}
