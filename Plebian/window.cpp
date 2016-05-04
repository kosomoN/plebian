#include "include\renderer\window.h"
#include <stdlib.h>
#include <stdio.h>
#include <GLFW\glfw3.h>
#include "include\globalsystem.h"

GlobalSystem* gSys;

Window::Window() {}


Window::~Window() {}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void Window::Create() {

	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	gSys = new GlobalSystem;
	gSys->p_window = window;
	gSys->Init();

	while (!glfwWindowShouldClose(window)) {
		gSys->Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return;
}
