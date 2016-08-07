#ifndef SHADER_H_
#define SHADER_H_

#include <string>

#include <GL/glew.h>

class Shader {
public:
	bool Init(std::string name);

	GLuint m_shader_program = 0;
private:
	GLuint LoadShader(std::string content, GLenum shader_type);
};

#endif // SHADER_H_
