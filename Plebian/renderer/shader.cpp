#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <gl/glew.h>

#include "globalsystem.h"

bool Shader::Init(std::string name) {
	std::string shader_file = LoadTextFile(name);
	if (shader_file.empty()) {
		LogError("Empty shader file: %s", name.c_str());
		return false;
	}
	size_t split_pos = shader_file.find("@");
	std::string vertex_content = shader_file.substr(0, split_pos);
	std::string fragment_content = shader_file.substr(split_pos + 1);
	const char* v = vertex_content.c_str();
	const char* f = fragment_content.c_str();
	
	//Create shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &v, NULL);
	glCompileShader(vs);
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint log_length = 0;
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(vs, log_length, &log_length, &log[0]);
		LogError("Error compiling vert shader: %s\n%s", name, log.data());
		glDeleteShader(vs);
		return false;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &f, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint log_length = 0;
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(fs, log_length, &log_length, &log[0]);
		LogError("Error compiling frag shader: %s\n%s", name, log);
		glDeleteShader(fs);
		glDeleteShader(vs);
		return false;
	}

	m_shader_program = glCreateProgram();
	glAttachShader(m_shader_program, fs);
	glAttachShader(m_shader_program, vs);
	glLinkProgram(m_shader_program);
	glGetProgramiv(m_shader_program, GL_LINK_STATUS, &status);
	if (!status) {
		GLint log_length;
		glGetProgramiv(m_shader_program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> log(log_length);
		glGetProgramInfoLog(m_shader_program, log_length, &log_length, &log[0]);
		LogError("Error linking shader: %s\n%s", name, log.data());
		glDeleteProgram(m_shader_program);
		m_shader_program = 0;
		glDeleteShader(vs);
		glDeleteShader(fs);
		return false;
	}
	glDetachShader(m_shader_program, vs);
	glDetachShader(m_shader_program, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return true;
}

std::string Shader::LoadTextFile(std::string file_path) {
	std::ifstream file(file_path);
	if (!file.is_open()) {
		LogError("Error opening file: %s", file_path.c_str());
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}
