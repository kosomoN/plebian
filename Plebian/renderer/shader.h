#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include <gl/glew.h>

class Shader {
public:
	bool Init(std::string name);

	GLuint m_shader_program = 0;
private:
	std::string LoadTextFile(std::string path);
};

#endif // SHADER_H_
