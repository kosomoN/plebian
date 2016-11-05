#ifndef _CUBE_RENDERER_H
#define _CUBE_RENDERER_H

#include <io/textureloader.h>

#include "texture.h"
#include "mesh_renderer.h"
#include "camera.h"
#include "environment_probe.h"

class CubeRenderer {
public:
	CubeRenderer(MeshRenderer* renderer, Camera& lightCamera, TextureLoader& textureLoader, int w, int h);
	~CubeRenderer();

	Texture* SampleCubemap(EnvironmentProbe* probe);
private:
	MeshRenderer* m_p_renderer;
	Camera m_lightCamera;
	Camera* m_p_renderCamera;
	TextureLoader m_textureLoader;
	int m_w, m_h;

};

#endif

