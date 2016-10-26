#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#define LERP_HISTORY_LENGTH 3

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <entityx/entityx.h>

struct Transform : entityx::Component<Transform> {
    glm::vec3 pos;
    Transform* parent = nullptr;
    glm::quat orientation;

    glm::mat4 WorldSpace();

private:

    void ParentWorldSpace(glm::mat4& child);
};


struct TransformHistoryComponent
    : entityx::Component<TransformHistoryComponent> {
    struct State {
        glm::vec3 pos;
        glm::quat orientation;
        int timestamp;
    };

    TransformHistoryComponent();

    void AddState(glm::vec3 pos, glm::quat orientation, int timestamp);
    void ReadState(float lerp_time, glm::vec3& pos, glm::quat& orientation);
private:
    State states[LERP_HISTORY_LENGTH];

};


#endif // TRANSFORM_H_
