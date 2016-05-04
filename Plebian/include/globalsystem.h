#ifndef GLOBALSYSTEM_H_
#define GLOBALSYSTEM_H_
#include <GLFW\glfw3.h>
#include <string>
#include <time.h>
#include <iostream>

struct GlobalSystem {
	GlobalSystem::GlobalSystem();
	GlobalSystem::~GlobalSystem();

	GLFWwindow* p_window;

	void Init();
	void Update();

	template <class T>
	void Log(std::string text, const T& obj);
	template <class T>
	void Error(std::string text, const T& obj);
};

extern GlobalSystem* gSys;


#endif

