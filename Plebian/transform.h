#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#define LERP_HISTORY_LENGTH 3

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entityx/entityx.h>
#include <ReplicaManager3.h>

#include "components.h"

struct Transform : entityx::Component<Transform>, public NetworkedComponent {
    glm::vec3 pos;
    Transform* parent = nullptr;
    glm::quat orientation;

    glm::mat4 WorldSpace();

    virtual uint8_t NetworkID();
    virtual void Serialize(RakNet::SerializeParameters *serialize_parameters);
    virtual bool Deserialize(RakNet::DeserializeParameters *deserialize_parameters, PlebianGame* game);
private:
    void ParentWorldSpace(glm::mat4& child);
};


struct TransformHistoryComponent
    : entityx::Component<TransformHistoryComponent> {
    struct State {
        glm::vec3 pos;
        glm::quat orientation;
        uint32_t timestamp;
        bool initialized = false;
    };

    void AddState(glm::vec3 pos, glm::quat orientation, uint32_t timestamp);
    bool DeserializeState(RakNet::DeserializeParameters *deserialization_parameters);
    void ReadState(float lerp_time, glm::vec3& pos, glm::quat& orientation);
private:
    State states[LERP_HISTORY_LENGTH];

};


#endif // TRANSFORM_H_
