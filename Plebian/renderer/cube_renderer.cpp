#include "cube_renderer.h"

#include <glm/glm.hpp>

#include "renderer/g_buffer.h"
#include "log.h"

#define CUBEMAP_SIZE 256

bool CubeRenderer::Init()
{
    camera.InitPerspective(CUBEMAP_SIZE, CUBEMAP_SIZE, 90.f);
    glGenFramebuffers(1, &fbo);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        Log(Error, "Framebuffer error: 0x%x", status);
        return false;
    }

    if (!g_buffer.Init(CUBEMAP_SIZE, CUBEMAP_SIZE))
        return false;

    return true;
}

void CubeRenderer::UpdateCubemap(EnvironmentProbe* probe)
{
	camera.transform.pos = probe->pos;

    // rotate all side views 180 degrees because of OpenGL's weird UVs
    static const glm::quat orientations[6] = {
        glm::angleAxis(glm::radians(180.f), glm::vec3(0, 0, 1))
        * glm::angleAxis(glm::radians( 90.f), glm::vec3(0, 1, 0)),

        glm::angleAxis(glm::radians(180.f), glm::vec3(0, 0, 1))
        * glm::angleAxis(glm::radians(-90.f), glm::vec3(0, 1, 0)),

        glm::angleAxis(glm::radians(-90.f), glm::vec3(1, 0, 0)),
        glm::angleAxis(glm::radians( 90.f), glm::vec3(1, 0, 0)),

        glm::angleAxis(glm::radians(180.f), glm::vec3(0, 0, 1))
        * glm::angleAxis(glm::radians(180.f), glm::vec3(0, 1, 0)),

        glm::angleAxis(glm::radians(180.f), glm::vec3(0, 0, 1))
        * glm::quat()
    };

	for (int i = 0; i < 6; i++) {
        camera.transform.orientation = orientations[i];
        camera.UpdateMatrix();

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, probe->cubemap->texture_id, 0);

        game->RenderFrame(camera, fbo, &g_buffer);
	}
}
