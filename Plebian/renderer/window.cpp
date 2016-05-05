#include "renderer/window.h"
#include <stdlib.h>
#include <stdio.h>
#include "globalsystem.h"

GlobalSystem* g_sys;

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
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		LogError(NULL, "Error: %s\n", glewGetErrorString(err));
	}

	g_sys = new GlobalSystem;
	g_sys->p_window = window;
	g_sys->Init();

	while (!glfwWindowShouldClose(window)) {
		g_sys->Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return;
}
