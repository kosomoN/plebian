#include "cube_renderer.h"

#include <glm/glm.hpp>

CubeRenderer::CubeRenderer(MeshRenderer* renderer, Camera& lightCamera, TextureLoader& textureLoader, int w, int h)
{
	m_lightCamera = lightCamera;
	m_p_renderer = renderer;
	m_p_renderCamera = new Camera();
	m_p_renderCamera->InitPerspective(w, h, 90.f);
	m_w = w;
	m_h = h;
	m_textureLoader = textureLoader;
}

CubeRenderer::~CubeRenderer()
{
	if (m_p_renderCamera)
		delete m_p_renderCamera;
}

Texture* CubeRenderer::SampleCubemap(EnvironmentProbe* probe)
{
	glm::vec3 pos = probe->GetPos();
	glm::mat4 views[6] = {
		glm::lookAt(pos, pos + glm::vec3(1,0,0), glm::vec3(0,-1,0)),
		glm::lookAt(pos, pos + glm::vec3(-1,0,0), glm::vec3(0,-1,0)),
		glm::lookAt(pos, pos + glm::vec3(0,1,0), glm::vec3(0,0,1)),
		glm::lookAt(pos, pos + glm::vec3(0,-1,0), glm::vec3(0,0,-1)),
		glm::lookAt(pos, pos + glm::vec3(0,0,1), glm::vec3(0,-1,0)),
		glm::lookAt(pos, pos + glm::vec3(0,0,-1), glm::vec3(0,-1,0))
	};

	const int width = 256;
	const int height = 256;

	unsigned char*** data[1][6][width * height];

	for (int i = 0; i < 6; i++) {
		m_p_renderCamera->UpdateMatrix(&views[i]);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, m_w, m_h, GL_RGB, GL_UNSIGNED_BYTE, &data[0][i]);
	}

	return nullptr;
}
