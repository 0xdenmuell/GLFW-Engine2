#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <camera/camera.h>
#include <shader/shader.h>

#include <iostream>

#define LOG(msg) std::cout << msg << std::endl;


void error_callback(int error, const char* description);

bool timer(double duration, const char* modus);

void processInput(GLFWwindow* window);

void window_size_callback(GLFWwindow* window, int width, int height);

float SCR_WIDTH = 1920;
float SCR_HEIGHT = 1080;

float mixValue = 0.5f;

Camera cam;

int main(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW Window", NULL, NULL);
	if (!window)
	{
		LOG("Failed to initialize GLFW")
			glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG("Failed to initialize GLAD");
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	Shader ShaderProgram("E:\\dev\\GLFW-Engine2\\Engine\\vertices.glsl",
		"E:\\dev\\GLFW-Engine2\\Engine\\fragment.glsl");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
	};


	//bind buffer
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load("E:\\dev\\GLFW-Engine2\\Engine\\container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("E:\\dev\\GLFW-Engine2\\Engine\\shrek.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ShaderProgram.use();
	ShaderProgram.setInt("texture1", 0);
	ShaderProgram.setInt("texture2", 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glfwSetFramebufferSizeCallback(window, window_size_callback);

	glfwSetWindowSizeCallback(window, window_size_callback);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 newModel = glm::mat4(1.0f);
			newModel = glm::translate(newModel, cubePositions[i]);
			float angle = 20.0f * i;
			newModel = glm::rotate(newModel, glm::radians(angle) * i, glm::vec3(1.0f, 0.3f, 0.5f));
			ShaderProgram.setMatrix4("model", newModel);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		ShaderProgram.setMatrix4("view", cam.getView());
		ShaderProgram.setMatrix4("projection", cam.getProjection());
		ShaderProgram.setFloat("mixValue", mixValue);
		ShaderProgram.use();

		glfwSwapBuffers(window);
		glfwPollEvents();
		// Keep running
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


void error_callback(int error, const char* description)
{
	std::cout << "ERROR::GLFW::error_callback" << error << ": " << description;
}


float completedTime = 0.0f;

bool timer(double duration, const char* modus)
{
	if (modus == "stopwatch")
	{
		if (completedTime == 0.0f)
		{
			glfwSetTime(0);
			LOG("TIMER BEGINS");
		}
		completedTime = glfwGetTime();
		if (duration <= completedTime)
		{
			LOG("TIMER ENDS");
			completedTime = 0.0f;
			return true;
		}
		return false;
	}

	else if (modus == "stop")
	{
		glfwSetTime(0);
		do {} while (glfwGetTime() <= duration);
		return true;
	}
	else {
		LOG("TIMER FUNCTION NO SET MODUS");
		return false;
	}

}

void processInput(GLFWwindow* window)
{
	//Quit Application
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.setCamPos(cam.getCamPos() + (cam.getCamSpeed() * cam.getCamTarget()));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.setCamPos(cam.getCamPos() - (cam.getCamSpeed() * cam.getCamTarget()));
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.setCamPos(cam.getCamPos() - glm::normalize(glm::cross(cam.getCamTarget(), cam.getUpVector())) * cam.getCamSpeed());
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.setCamPos(cam.getCamPos() + glm::normalize(glm::cross(cam.getCamTarget(), cam.getUpVector())) * cam.getCamSpeed());
	}
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
	LOG(SCR_HEIGHT); LOG(SCR_WIDTH);
	glViewport(0, 0, SCR_HEIGHT, SCR_WIDTH);
}
