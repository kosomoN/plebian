#include "transform.h"

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


TransformHistoryComponent::TransformHistoryComponent() {
    for (int i = 0; i < LERP_HISTORY_LENGTH; i++)
        states[i].timestamp = -1;
}

void TransformHistoryComponent::AddState(glm::vec3 pos, glm::quat orientation, int timestamp) {
    if (timestamp < states[0].timestamp) return;

    for (int i = LERP_HISTORY_LENGTH - 1; i > 0; i--) {
        states[i] = states[i - 1];
    }
    states[0].pos = pos;
    states[0].orientation = orientation;
    states[0].timestamp = timestamp;
}

void TransformHistoryComponent::ReadState(float lerp_time, glm::vec3& pos, glm::quat& orientation) {
    State lerp_state;

    for (int i = LERP_HISTORY_LENGTH - 1; i >= 0; i--) {
        unsigned int timestamp = states[i].timestamp;

        // uninitialized state
        if (timestamp == -1) continue;

        if (timestamp > lerp_time) {

            // no old enough state, return oldest
            if (i + 1 >= LERP_HISTORY_LENGTH) {
                pos = states[LERP_HISTORY_LENGTH - 1].pos;
                orientation = states[LERP_HISTORY_LENGTH - 1].orientation;
                return;
            }

            // interpolate between state i and i + 1
            unsigned int old_timestamp = states[i + 1].timestamp;

            // the older one was invalid
            if (old_timestamp == -1) {
                pos = states[i].pos;
                orientation = states[i].orientation;
                return;
            }


            float percentage = (lerp_time - old_timestamp)
                / (timestamp - old_timestamp);

            pos = glm::mix(states[i + 1].pos,
                                       states[i].pos,
                                       percentage);
            orientation = glm::slerp(states[i + 1].orientation,
                                                states[i].orientation,
                                                percentage);
            return;
        }
    }

    // return latest one
    pos = states[0].pos;
    orientation = states[0].orientation;
}