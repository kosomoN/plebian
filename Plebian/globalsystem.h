#ifndef GLOBALSYSTEM_H_
#define GLOBALSYSTEM_H_
#include <string>
#include <iostream>
#include <stdarg.h>
#include <time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct GlobalSystem {
	GlobalSystem::GlobalSystem() {};
	GlobalSystem::~GlobalSystem() {};

	GLFWwindow* p_window;

	void Init() {};
	void Update() {};
};

extern GlobalSystem* g_sys;

#endif

