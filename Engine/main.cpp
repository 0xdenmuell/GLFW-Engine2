#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdlib.h>

#define LOG(msg) std::cout << msg << std::endl;

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


	if (key != 0 && action == GLFW_PRESS)
	{
		std::cout << glfwGetKeyName(key, scancode);
	}
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


int main(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(800, 800, "GLFW Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);

	glViewport(0, 0, 800, 800);

	glfwSetFramebufferSizeCallback(window, window_size_callback);


	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();		
		// Keep running
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}