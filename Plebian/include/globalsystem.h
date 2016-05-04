#ifndef GLOBALSYSTEM_H_
#define GLOBALSYSTEM_H_
#include <GLFW/glfw3.h>
#include <string>
#include <time.h>
#include <iostream>

struct GlobalSystem {
	GlobalSystem::GlobalSystem() {};
	GlobalSystem::~GlobalSystem() {};

	GLFWwindow* p_window;

	void Init() {};
	void Update() {};

	template <class T>
	void LogInfo(const T& obj, const char* format, ...);
	template <class T>
	void LogWarn(const T& obj, const char* format, ...);
	template <class T>
	void LogError(const T& obj, const char* format, ...);
};

extern GlobalSystem* g_sys;


#endif

