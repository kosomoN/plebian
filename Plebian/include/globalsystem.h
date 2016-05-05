#ifndef GLOBALSYSTEM_H_
#define GLOBALSYSTEM_H_
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <stdarg.h>
#include <time.h>

struct GlobalSystem {
	GlobalSystem::GlobalSystem() {};
	GlobalSystem::~GlobalSystem() {};

	GLFWwindow* p_window;

	void Init() {};
	void Update() {};
};

extern GlobalSystem* g_sys;

void LogInfo(const char* format, ...);
void LogWarn(const char* format, ...);
void LogError(const char* format, ...);

#endif

