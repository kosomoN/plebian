#ifndef _CUBE_RENDERER_H
#define _CUBE_RENDERER_H

#include <io/textureloader.h>

#include "mesh_renderer.h"
#include "camera.h"
#include "environment_probe.h"

class CubeRenderer {
public:
	CubeRenderer(PlebianGame* game);

    void UpdateCubemap(EnvironmentProbe* probe);
private:
    Camera camera;
    PlebianGame* game = nullptr;
};

#endif

