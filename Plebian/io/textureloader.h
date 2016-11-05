#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include "renderer/texture.h"

typedef unsigned char**** CubemapData;

class TextureLoader {
public:
	Texture* GetTexture2d(std::string name);
	Texture* Create2dTexture(int width, int height, unsigned char** data);
	Texture* Create3dTexture(int mips, int width, int height, CubemapData data);
	Texture* GetTexture3d(std::string** filename, int mips);
private:
	std::unordered_map<std::string, Texture*> m_textures;
};

#endif

