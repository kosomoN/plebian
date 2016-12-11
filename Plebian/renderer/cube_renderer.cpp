#include "cube_renderer.h"

#include <glm/glm.hpp>

CubeRenderer::CubeRenderer(MeshRenderer* renderer, Camera& lightCamera, TextureLoader& textureLoader)
{
	renderer = renderer;
	camera.InitPerspective(1, 1, 90.f);
}

void CubeRenderer::UpdateCubemap(EnvironmentProbe* probe)
{
	camera.transform.pos == probe->pos;

    static const glm::quat orientations[6] = {
        glm::angleAxis( 90.f, glm::vec3(0, 1, 0)),
        glm::angleAxis(-90.f, glm::vec3(0, 1, 0)),
        glm::angleAxis( 90.f, glm::vec3(1, 0, 0)),
        glm::angleAxis(-90.f, glm::vec3(1, 0, 0)),
        glm::angleAxis(180.f, glm::vec3(0, 1, 0)),
        glm::quat()
    };

	for (int i = 0; i < 6; i++) {
        camera.transform.orientation = orientations[i];
        camera.UpdateMatrix();
	}
}
