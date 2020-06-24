#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/string_cast.hpp>

#include <camera/camera.h>
#include <shader/shader.h>

#include <iostream>

#define LOG(msg) std::cout << msg << std::endl


void error_callback(int error, const char* description);
bool timer(double duration, const char* modus);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float SCR_WIDTH = 1920;
float SCR_HEIGHT = 1080;

Camera cam(glm::vec3(0.0f, 1.0f, 0.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool debug;


glm::vec3 cubePositions[] = {
glm::vec3(2.0f,  0.0f, -5.0f),
glm::vec3(0.0f,  0.0f,  0.0f),
};

int main(void)
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Enable VSync
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		LOG("Failed to initialize GLFW");
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	Shader cubeProgram("E:\\dev\\GLFW-Engine2\\Engine\\cubeVertices.glsl",
		"E:\\dev\\GLFW-Engine2\\Engine\\cubeFragment.glsl");

	Shader lightProgram("E:\\dev\\GLFW-Engine2\\Engine\\lightVertices.glsl",
		"E:\\dev\\GLFW-Engine2\\Engine\\lightFragment.glsl");

	float vertices[] = {
		// position				// unit
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f
	};



	//bind buffer
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// unit attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(lightVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glfwSetFramebufferSizeCallback(window, window_size_callback);

	glfwSetWindowSizeCallback(window, window_size_callback);
	glEnable(GL_DEPTH_TEST);

	float currentFrame;

	while (!glfwWindowShouldClose(window))
	{


		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//CUBE
		cubeProgram.use();

		cubeProgram.setVec3("objectColor", objectColor);
		cubeProgram.setVec3("lightColor", lightColor);
		cubeProgram.setVec3("lightPos", glm::vec3(cubePositions[1].x, cubePositions[1].y, cubePositions[1].z));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[0]);

		cubeProgram.setMatrix4("model", model);
		cubeProgram.setMatrix4("view", cam.getView());
		cubeProgram.setMatrix4("projection", cam.getProjection());

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//LIGHT
		lightProgram.use();

		lightProgram.setVec3("lightColor", lightColor);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[1]);

		lightProgram.setMatrix4("model", model);
		lightProgram.setMatrix4("view", cam.getView());
		lightProgram.setMatrix4("projection", cam.getProjection());

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
		// Keep running
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

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

	//MOVEMENT
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.moveCam(FORWARD);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.moveCam(BACKWARDS);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.moveCam(LEFT);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.moveCam(RIGHT);
	}

	//SPRINT
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cam.setSpeed(10.0f);
	}
	else {
		cam.setSpeed(2.5f);
	}

	//ZOOM
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		//Zoom In
		cam.setFOV(20.0f);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		//Zoom Out
		cam.setFOV(90.0f);
	}

	//DEBUG
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cubePositions[0].z += 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cubePositions[0].z -= 0.1;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		if (!debug)
		{			
		}
		else
		{		
		}
	}
}


float lastX = 400;
float lastY = 300;
float yaw = 90.0f;
float pitch = 0;

void mouse_callback(GLFWwindow* window, double posX, double posY) {

	float offsetX = posX - lastX;
	float offsetY = lastY - posY;
	lastX = posX;
	lastY = posY;

	const float sensitivity = 0.1f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	yaw += offsetX;
	pitch += offsetY;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cam.setCamTarget(glm::normalize(direction));
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
	LOG("Höhe: " << SCR_HEIGHT); LOG("Breite: " << SCR_WIDTH);
	glViewport(0, 0, SCR_HEIGHT, SCR_WIDTH);
}