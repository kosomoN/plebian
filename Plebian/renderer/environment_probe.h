#ifndef ENVIRONMENT_PROBE_H
#define ENVIRONMENT_PROBE_H

#include <../inc/glm/vec3.hpp>
#include "texture.h"
#include "mesh_renderer.h"
#include "camera.h"

class EnvironmentProbe {
public:
	EnvironmentProbe(int radius) {}

	glm::vec3&  GetPos() { return m_position; }
	void SetPos(glm::vec3& pos) { m_position = pos; }
	Texture* GetCubemap() { return m_cubemap; }
	glm::vec3& GetArea() { return m_area; }
	glm::vec3 SetArea(glm::vec3& r) { m_area = r;  }

private:
	glm::vec3 m_position;
	Texture* m_cubemap;
	glm::vec3 m_area;
};

#endif