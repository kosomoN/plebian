#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <../inc/glm/vec3.hpp>
#include "texture.h"

class EnvironmentProbe {
public:
	EnvironmentProbe(int radius) {}

	glm::vec3&  GetPos() { return m_position; }
	void SetPos(glm::vec3& pos) { m_position = pos; }
	Texture* GetCubemap() { return m_cubemap; }
	int& GetRadius() { return m_radius; }
	void SetRadius(int r) { m_radius = r;  }
	void Generate(int width, int height);

private:
	glm::vec3 m_position;
	Texture* m_cubemap;
	int m_radius;
};

#endif