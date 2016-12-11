#ifndef _CUBE_RENDERER_H
#define _CUBE_RENDERER_H

#include <io/textureloader.h>

#include "mesh_renderer.h"
#include "camera.h"
#include "environment_probe.h"

class CubeRenderer {
public:
	CubeRenderer(MeshRenderer* renderer, Camera& lightCamera, TextureLoader& textureLoader);

    void UpdateCubemap(EnvironmentProbe* probe);
private:
	MeshRenderer* renderer;
	Camera camera;
};

#endif

