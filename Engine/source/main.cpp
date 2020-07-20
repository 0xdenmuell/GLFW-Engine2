#include <precom.h>

#include <glad/glad.h>
#include <glfw/include/GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/string_cast.hpp>

#include <camera/camera.h>
#include <shader/shader.h>
#include <shader/model.h>
#include <window/window.h>


bool timer(double duration, const char* modus);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Camera cam;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool debug;

float cutOff = 12.5;
float outerCutOff = 17.5;

int main(void)
{
	//Creates a Window with all properties
	Window window;

	if (!window.m_fail)
	{
		LOG("Created Game Window!");
	}
	else {
		LOG("Failed to Create Game Window!");
		return false;
	}



	glfwSetInputMode(window.m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window.m_GLFWwindow, mouse_callback);
	glfwSetKeyCallback(window.m_GLFWwindow, key_callback);

	Shader cubeProgram(MODEL);
	Model object(BACKPACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	float currentFrame;


	while (!glfwWindowShouldClose(window.m_GLFWwindow))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window.m_GLFWwindow);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeProgram.use();

		//LOAD DIFFRENT LIGHT SOURCE

		//cubeProgram.loadDirLight();

		//PERSPECTIVES

		cam.UpdateValues();

		glm::mat4 model = model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		cubeProgram.setMatrix4("model", model);

		cubeProgram.setMatrix4("view", cam.getView());
		cubeProgram.setMatrix4("projection", cam.getProjection());

		object.Draw(cubeProgram);

		glfwSwapBuffers(window.m_GLFWwindow);
		glfwPollEvents();
		// Keep running
	}

	glfwDestroyWindow(window.m_GLFWwindow);
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
//to do: enum codes
{
	//Quit Application
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	//MOVEMENT
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.setMovementPosition(FORWARD);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.setMovementPosition(BACKWARDS);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.setMovementPosition(LEFT);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.setMovementPosition(RIGHT);
	}

	//SPRINT
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cam.m_speed = HIGH;
	}
	else {
		cam.m_speed = LOW;
	}

	//ZOOM
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		//Zoom In
		cam.m_fov = ZOOM_IN;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		//Zoom Out
		cam.m_fov = ZOOM_OUT;
	}

	//DEBUG
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		cutOff += 1;
		LOG("cutOff: " << cutOff);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		cutOff -= 1;
		LOG("cutOff: " << cutOff);
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		outerCutOff += 1;
		LOG("outerCutOff: " << outerCutOff);
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		outerCutOff -= 1;
		LOG("outerCutOff: " << outerCutOff);
	}
}

void mouse_callback(GLFWwindow* window, double posX, double posY) {
	cam.setMovementTarget(posX, posY);
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	g_SCR_HEIGHT = height;
	g_SCR_WIDTH = width;
	LOG("Höhe: " << g_SCR_HEIGHT); LOG("Breite: " << g_SCR_WIDTH);
	glViewport(0, 0, g_SCR_HEIGHT, g_SCR_WIDTH);
}