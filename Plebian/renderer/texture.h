#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef SERVER

struct Texture {
    std::string name;
};

#else
#include <GL/glew.h>

struct Texture {
	GLuint texture_id;
};

#endif

#endif