#include "textureloader.h"

#include <lodepng\lodepng.hpp>

#include "../globalsystem.h"

#define TEXTURE_PATH "assets/textures/"

Texture* TextureLoader::GetTexture(std::string fn) {
	if (m_textures.count(fn))
		return m_textures[fn];

	unsigned int width, height;
	std::vector<unsigned char> data;

	auto e = lodepng::decode(data, width, height, fn.c_str());

	if (e != 0)
		LogError("Failed to load texture %s", fn.c_str());

	Texture* p_tex = new Texture;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &p_tex->textue_id);
	glBindTexture(GL_TEXTURE_2D, p_tex->textue_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	m_textures[fn] = p_tex;
	return p_tex;
}