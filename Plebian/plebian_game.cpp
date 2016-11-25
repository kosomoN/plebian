#include "plebian_game.h"

#include "network/network_defines.h"
#include "log.h"

#define RESET_TIME (500.0f / TICK_LENGTH_MS)

void PlebianGame::NewSnapshot(uint32_t snapshot_time)
{
    if (snapshot_time - last_received_snapshot > 1)
        Log(Warn, "%i Dropped", snapshot_time - last_received_snapshot);
    last_received_snapshot = snapshot_time;
    float new_server_delta = snapshot_time - (current_tick + current_tick_fraction);

    float delta_delta = new_server_delta - server_time_delta;

    // if the delta has changed a lot just reset it instead of trying to smooth it
    if (abs(delta_delta) > RESET_TIME) {
        Log(Warn, "Server delta reset");
        server_time_delta = new_server_delta;

    } else if (abs(delta_delta) > 100.0f / TICK_LENGTH_MS) {
        // adjust fast
        server_time_delta += delta_delta / 2.0f;
        Log(Warn, "Server delta quickly adjusted");

    } else {
        if (delta_delta > 0)
            server_time_delta += 1.0f / TICK_LENGTH_MS;
        else if (current_tick >= snapshot_time)
            server_time_delta -= 1.0f / TICK_LENGTH_MS;

    }
}
