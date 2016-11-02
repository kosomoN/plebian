#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include "renderer/texture.h"

typedef unsigned char**** CubemapData;

class TextureLoader {
public:
	Texture* GetTexture2d(std::string name, int width, int height, unsigned char** data = nullptr);
	Texture* GetTexture3d(std::string** name, int width, int height, int mips, CubemapData data = nullptr);
private:
	std::unordered_map<std::string, Texture*> m_textures;
};

#endif

