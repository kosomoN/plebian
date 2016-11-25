#ifndef PLEBIAN_GAME_H_
#define PLEBIAN_GAME_H_

#include <glm/glm.hpp>
#include <entityx/entityx.h>

struct PlebianGame {
    PlebianGame() : entity_manager(events) {}

    void NewSnapshot(uint32_t snapshot_time);

    entityx::EventManager events;
    entityx::EntityManager entity_manager;

    uint32_t current_tick = 0;
    // between 0 and 1, how far in between ticks the engine is
    float current_tick_fraction = 0.0f;
    uint32_t last_received_snapshot = 0;

    // difference between snapshot timestamp and client time when a snapshot is received
    float server_time_delta = 0.0f;
};

#endif //  PLEBIAN_GAME_H_
