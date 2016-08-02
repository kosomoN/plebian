#include "textureloader.h"

#include <lodepng/lodepng.hpp>

#include "log.h"

#define TEXTURE_PATH "assets/textures/"

Texture* TextureLoader::GetTexture(std::string fn) {
	if (m_textures.count(fn))
		return m_textures[fn];

	Log(Info, "Loading texture: %s", fn.c_str());

	unsigned int width, height;
	std::vector<unsigned char> data;

	unsigned int e = lodepng::decode(data, width, height, (TEXTURE_PATH + fn).c_str());

	if (e != 0) {
		Log(Error, "Failed to load texture %s", fn.c_str());
		return nullptr;
	}

	Texture* p_tex = new Texture;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &p_tex->texture_id);
	glBindTexture(GL_TEXTURE_2D, p_tex->texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	m_textures[fn] = p_tex;
	return p_tex;
}