#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <stdlib.h>

#define LOG(msg) std::cout << msg << std::endl;

void error_callback(int error, const char* description)
{
	std::cout << "ERROR::GLFW::error_callback" << error << ": " << description;
}

void timer(double duration) {
	glfwSetTime(0);
	do {} while (glfwGetTime() <= duration);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		return;
	}

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

	///compile errors
	int successCompile;
	char infoLogCompile[512];

	///create shader
	const char* vertexShaderSource = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int shaderID;
	shaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderID, 1, &vertexShaderSource, NULL);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &successCompile);

	if (!successCompile)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLogCompile);
		std::cout	<< "compilation error for shaderID: " << shaderID << std::endl
					<< "error log: " << infoLogCompile;
	}

	///create fragment
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0);\n"
		"}\0";

	unsigned int fragmentID;
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &successCompile);

	if (!successCompile)
	{
		char infoLogCompile[512];
		glGetShaderInfoLog(fragmentID, 512, NULL, infoLogCompile);
		std::cout	<< "compilation error for fragmentID: " << fragmentID << std::endl
					<< "error log: " << infoLogCompile;

	}

	///create linking program

	unsigned int shaderProgramID;
	shaderProgramID = glCreateProgram();

	glAttachShader(shaderProgramID, shaderID);
	glAttachShader(shaderProgramID, fragmentID);
	glLinkProgram(shaderProgramID);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &successCompile);

	if (!successCompile)
	{
		char infoLogCompile[512];
		glGetShaderInfoLog(shaderProgramID, 512, NULL, infoLogCompile);
		std::cout << "compilation error for shaderProgramID: " << shaderProgramID << std::endl
			<< "error log: " << infoLogCompile;

	}

	glDeleteShader(shaderID);
	glDeleteShader(fragmentID);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};


	//bind buffer
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, window_size_callback);

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramID);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
		// Keep running
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}