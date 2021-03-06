#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include "log.h"

#define SHADER_PATH "assets/shaders/"

bool Shader::Init(std::string name) {
	std::ifstream file(SHADER_PATH + name);
	if (!file.is_open()) {
		Log(Error, "Error opening file: %s", (SHADER_PATH + name).c_str());
		return false;
	}

	shader_program = glCreateProgram();

	std::vector<GLuint> shaders;
	std::stringstream shader_data;
	std::string line;
	GLenum shader_type = 0;
	while (std::getline(file, line)) {
		if (line[0] == '@') {
			if (shader_type != 0) {
				GLuint loaded_shader = LoadShader(shader_data.str(), shader_type);
				if (loaded_shader == 0) {
					for (GLuint shader : shaders) {
						glDetachShader(shader_program, shader);
						glDeleteShader(shader);
					}
					glDeleteProgram(shader_program);
					shader_program = 0;
					file.close();
					return false;
				}
				shaders.push_back(loaded_shader);
			}

			shader_data.str(std::string());
			if (line == "@vertex")
				shader_type = GL_VERTEX_SHADER;
			else if (line == "@fragment")
				shader_type = GL_FRAGMENT_SHADER;
			else if (line == "@geometry")
				shader_type = GL_GEOMETRY_SHADER;
			else if (line == "@tess_evaluation")
				shader_type = GL_TESS_EVALUATION_SHADER;
			else if (line == "@tess_control")
				shader_type = GL_TESS_CONTROL_SHADER;
		} else {
			shader_data << line << "\n";
		}
	}
	if (shader_type != 0) {
		GLuint loaded_shader = LoadShader(shader_data.str(), shader_type);
		if (loaded_shader == 0) {
			for (GLuint shader : shaders) {
				glDetachShader(shader_program, shader);
				glDeleteShader(shader);
			}
			glDeleteProgram(shader_program);
			shader_program = 0;
			file.close();
			return false;
		}
		shaders.push_back(loaded_shader);
	}

	if (shaders.empty()) {
		Log(Error, "No shaders loaded from %s", (SHADER_PATH + name).c_str());
		glDeleteProgram(shader_program);
		shader_program = 0;
		file.close();
		return false;
	}

	GLint status;
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (!status) {
		GLint log_length;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> log(log_length);
		glGetProgramInfoLog(shader_program, log_length, &log_length, &log[0]);
		Log(Error, "Error linking shader: %s\n%s", (SHADER_PATH + name).c_str(), log.data());
		for (GLuint shader : shaders) {
			glDetachShader(shader_program, shader);
			glDeleteShader(shader);
		}
		glDeleteProgram(shader_program);
		shader_program = 0;
		file.close();
		return false;
	}

	for (GLuint shader : shaders) {
		glDetachShader(shader_program, shader);
		glDeleteShader(shader);
	}
	file.close();

	return true;
}

GLuint Shader::LoadShader(std::string content, GLenum shader_type) {
	GLuint shader_id = glCreateShader(shader_type);
	const char* p_content = content.c_str();
	glShaderSource(shader_id, 1, &p_content, NULL);
	glCompileShader(shader_id);
	GLint status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint log_length = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(shader_id, log_length, &log_length, &log[0]);
		Log(Error, "Error compiling shader: %s", log.data());
		glDeleteShader(shader_id);
		return 0;
	}

	glAttachShader(shader_program, shader_id);
	return shader_id;
}

