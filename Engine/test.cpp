#include "include/GLFW/glfw3.h"
#include <iostream>
#include <stdlib.h>

void error_callback(int error, const char* description)
{
	std::cout << "Error " << error << ": " << description;
}

void timer(double duration) {
	glfwSetTime(0);
	do {} while (glfwGetTime() <= duration);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(void)
{
	glfwSetErrorCallback(error_callback);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(800, 800, "GLFW Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	int counter = 0;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Keep running
	}

	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}