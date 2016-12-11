#include "plebian_game.h"

#include <GL/glew.h>

#include "network/network_defines.h"
#include "log.h"
#include "renderer/mesh_renderer.h"
#include "renderer/g_buffer.h"
#include "renderer/light_system.h"

bool PlebianGame::Init()
{
    return default_shader.Init("basic.glsl");
}

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

void PlebianGame::UpdateServerTime()
{
    server_time = current_tick + current_tick_fraction + server_time_delta - lerp_amount;
}

void PlebianGame::RenderFrame(Camera& camera, GLuint output_fbo)
{
    glViewport(0, 0, g_buffer->width, g_buffer->height);
    glEnable(GL_DEPTH_TEST);

    g_buffer->Draw();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh_renderer->Render(camera, default_shader.shader_program);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    g_buffer->Read();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    light_system->LightPass(&camera);
}