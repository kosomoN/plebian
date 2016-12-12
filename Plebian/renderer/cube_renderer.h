#ifndef _CUBE_RENDERER_H
#define _CUBE_RENDERER_H

#include <io/textureloader.h>

#include "mesh_renderer.h"
#include "camera.h"
#include "environment_probe.h"
#include "renderer/g_buffer.h"

class CubeRenderer {
public:
    CubeRenderer(PlebianGame* game) : game(game) {}
    bool Init();

    void UpdateCubemap(EnvironmentProbe* probe);
private:
    Camera camera;
    GBuffer g_buffer;
    PlebianGame* game = nullptr;

    GLuint fbo = 0;
};

#endif

