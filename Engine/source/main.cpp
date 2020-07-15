#include <precom.h>

#include <glad/glad.h>
#include <glfw/include/GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/string_cast.hpp>

#include <camera/camera.h>
#include <shader/shader.h>
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
MaterialType material;

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

	Shader cubeProgram(CUBE);
	Shader lightProgram(LIGHT);

	glfwSetInputMode(window.m_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window.m_GLFWwindow, mouse_callback);
	glfwSetKeyCallback(window.m_GLFWwindow, key_callback);

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f, 0.0f),


		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(1.0f,  2.0f, 3.0f),
		glm::vec3(2.0f,  4.0f, 3.0f)
	};

	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	//bind buffer
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// unit attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(lightCubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);

	unsigned int diffuseMap = cubeProgram.loadTexture(WOODENBOX);
	unsigned int specularMap = cubeProgram.loadTexture(WOODENBOXFRAME);

	cubeProgram.use();
	cubeProgram.setInt("material.diffuseTexture", 0);
	cubeProgram.setInt("material.specularTexture", 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	float currentFrame;


	while (!glfwWindowShouldClose(window.m_GLFWwindow))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window.m_GLFWwindow);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//CUBE
		cubeProgram.use();

		//COLOR
		cubeProgram.setVec3("objectColor", objectColor);
		cubeProgram.setVec3("viewPos", cam.m_position);

		//MATERIAL & TEXTURE 
		cubeProgram.setMaterial(material);

		//LIGHT STRENGTH OF CUBE
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		cubeProgram.setVec3("light.ambient", ambientColor);
		cubeProgram.setVec3("light.diffuse", diffuseColor);
		cubeProgram.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		//LOAD DIFFRENT LIGHT SOURCE

		//cubeProgram.loadDirLight();

		for (int i = 0; i < 4; i++)
		{
			cubeProgram.loadPointLight(pointLightPositions[i], i);
		}

		cubeProgram.loadSpotLight(cam.m_position, cam.m_target);



		//PERSPECTIVES
		for (unsigned int i = 1; i < 10; i++)
		{
			glm::mat4 model = model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 0.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeProgram.setMatrix4("model", model);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		cubeProgram.setMatrix4("view", cam.getView());
		cubeProgram.setMatrix4("projection", cam.getProjection());

		cam.UpdateValues();

		//ASSIGN TEXTURE TO BUFFER
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);




		//LIGHTCUBE
		lightProgram.use();

		lightProgram.setVec3("lightColor", lightColor);


		for (unsigned int i = 1; i < 4; i++)
		{
			glm::mat4 model = model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			float angle = 0.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightProgram.setMatrix4("model", model);

			glBindVertexArray(lightCubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lightProgram.setMatrix4("view", cam.getView());
		lightProgram.setMatrix4("projection", cam.getProjection());


		glfwSwapBuffers(window.m_GLFWwindow);
		glfwPollEvents();
		// Keep running
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

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

	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

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