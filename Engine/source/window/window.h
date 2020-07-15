#pragma once
#include <glad/glad.h>
#include <glfw/include/GLFW/glfw3.h>


#include <precom.h>

float g_SCR_WIDTH;
float g_SCR_HEIGHT;

class Window
{
public:
	GLFWwindow* m_GLFWwindow;
	bool m_fail = false;

	Window(float width = 1920, float height = 1080, std::string windowName = "OpenGL window", GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL) {

		/*------------------------------------------------------------------------------------------
		Create Window
		------------------------------------------------------------------------------------------*/

		void error_callback(int error, const char* description);

		//Assign Value to struct GameWindow
		g_SCR_WIDTH = width;
		g_SCR_HEIGHT = height;
		windowName = windowName;

		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) {
			LOG("Failed to initialize GLFW");
			m_fail = true;
		}

		//Set GLFW Version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//Create GLFW Window
		m_GLFWwindow = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
		if (!m_GLFWwindow)
		{
			LOG("Failed to initialize GLFW");
			glfwTerminate();
			m_fail = true;
		}

		//Assin all states to new created window
		glfwMakeContextCurrent(m_GLFWwindow);

		//Initalize GLAD
		//TODO: Get GLAD out of window class
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG("Failed to initialize GLAD");
			m_fail = true;
		}

		glViewport(0, 0, width, height);
	}

private:
	
	std::string windowName;
	

};
