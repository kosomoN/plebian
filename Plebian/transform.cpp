#include "transform.h"

#include "log.h"
#include "network/network_defines.h"

glm::mat4 Transform::WorldSpace() {
    glm::mat4 worldSpace;
    ParentWorldSpace(worldSpace);
    return worldSpace;
}

void Transform::ParentWorldSpace(glm::mat4& child) {
    child = glm::mat4_cast(orientation) * child;
    child = glm::translate(glm::mat4(), pos) * child;
    if (parent)
        parent->ParentWorldSpace(child);
}

void Transform::Serialize(RakNet::SerializeParameters *ser)
{
    ser->outputBitstream->WriteVector(pos.x, pos.y, pos.z);
    ser->outputBitstream->WriteNormQuat(orientation.x, orientation.y, orientation.z, orientation.w);
}

bool Transform::Deserialize(RakNet::DeserializeParameters *deser)
{
    if (!deser->serializationBitstream->ReadVector(pos.x, pos.y, pos.z)) return false;
    if (!deser->serializationBitstream->ReadNormQuat(orientation.x, orientation.y, orientation.z, orientation.w)) return false;
    return true;
}

void TransformHistoryComponent::AddState(glm::vec3 pos, glm::quat orientation, uint32_t timestamp) {
    if (timestamp < states[0].timestamp) return;

    for (int i = LERP_HISTORY_LENGTH - 1; i > 0; i--) {
        states[i] = states[i - 1];
    }
    states[0].pos = pos;
    states[0].orientation = orientation;
    states[0].timestamp = timestamp;
    states[0].initialized = true;
}

bool TransformHistoryComponent::DeserializeState(RakNet::DeserializeParameters *deser, uint32_t timestamp)
{
    glm::vec3 pos;
    glm::quat orientation;
    if (!deser->serializationBitstream->ReadVector(pos.x, pos.y, pos.z)) return false;
    if (!deser->serializationBitstream->ReadNormQuat(orientation.x, orientation.y, orientation.z, orientation.w)) return false;

    AddState(pos, orientation, timestamp);

    return true;
}

void TransformHistoryComponent::ReadState(float lerp_time, glm::vec3& pos, glm::quat& orientation) {
    State lerp_state;

    for (int i = LERP_HISTORY_LENGTH - 1; i >= 0; i--) {
        unsigned int timestamp = states[i].timestamp;

        // uninitialized state
        if (!states[i].initialized) continue;

        // if this is the newest and it is still not new enough, extrapolate
        if (timestamp > lerp_time || i == 0) {

            // no old enough state, return oldest
            if (i + 1 >= LERP_HISTORY_LENGTH) {
                pos = states[LERP_HISTORY_LENGTH - 1].pos;
                orientation = states[LERP_HISTORY_LENGTH - 1].orientation;
                Log(Warn, "Ran out of data to lerp");
                return;
            }

            // the older one was invalid
            if (!states[i + 1].initialized) {
                pos = states[i].pos;
                orientation = states[i].orientation;
                return;
            }

            // interpolate/extrapolate between state i and i + 1
            unsigned int old_timestamp = states[i + 1].timestamp;

            // clamp extrapolation to 250 ms
            float percentage = (min(timestamp + 250.f / TICK_LENGTH_MS, lerp_time) - old_timestamp)
                                    / (timestamp - old_timestamp);

            if (lerp_time > timestamp) {
                Log(Warn, "Extrapolating %i%%", (int)(percentage * 100.0f));
            }

            pos = glm::mix(states[i + 1].pos,
                                       states[i].pos,
                                       percentage);
            orientation = glm::slerp(states[i + 1].orientation,
                                                states[i].orientation,
                                                percentage);
            return;
        }
    }
}
