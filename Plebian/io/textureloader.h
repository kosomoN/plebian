#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>

#include "renderer/texture.h"

class TextureLoader {
public:
	Texture* GetTexture(std::string name);
private:
	std::unordered_map<std::string, Texture*> m_textures;
};

#endif

