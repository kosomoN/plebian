#include "textureloader.h"

#include <lodepng/lodepng.hpp>

#include "log.h"

#define TEXTURE_PATH "assets/textures/"

Texture* TextureLoader::GetTexture2d(std::string fn) 
{
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	m_textures[fn] = p_tex;
	return p_tex;
}

Texture * TextureLoader::Create2dTexture(int width, int height, unsigned char ** data)
{
	return nullptr;
}

Texture * TextureLoader::Create3dTexture(int mips, int width, int height, CubemapData data)
{
	Texture* p_tex = new Texture;
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glGenTextures(1, &p_tex->texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, p_tex->texture_id);

	for (int i = 0; i < mips; ++i) {
		for (int j = 0; j < 6; ++j) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, i, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[i][j]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
	}

	std::string key = std::to_string(rand() * 99999);
	while (m_textures[key]) {
		key = std::to_string(rand() * 99999);
	}
	m_textures[key] = p_tex;
	return p_tex;
}

Texture* TextureLoader::GetTexture3d(std::string** filename, int mips)
{
	Texture* p_tex = new Texture;
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glGenTextures(1, &p_tex->texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, p_tex->texture_id);

	for (int i = 0; i < mips; ++i) {
		for (int j = 0; j < 6; ++j) {
			std::vector<unsigned char> data;
			unsigned int w, h;

			unsigned int e = lodepng::decode(data, w, h, (TEXTURE_PATH + filename[i][j]).c_str());

			if (e != 0) {
				Log(Error, "Failed to load texture %s", filename[i][j].c_str());
				return nullptr;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_SRGB_ALPHA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
	}
	m_textures[filename[0][0]] = p_tex;
	return p_tex;
}