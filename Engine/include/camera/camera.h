#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>

extern float SCR_WIDTH;
extern float SCR_HEIGHT;
extern float deltaTime;

#define LOG(msg) std::cout << msg << std::endl
#define vec3LOG(vec3) std::cout << vec3.x << "|" << vec3.y << "|" << vec3.z << std::endl
#define vec4LOG(vec3) std::cout << vec3.x << "|" << vec3.y << "|" << vec3.z << "|" << vec3.w << std::endl


enum camDirections {
	FORWARD = 1,
	BACKWARDS = 2,
	LEFT = 3,
	RIGHT = 4
};

enum cameraZoom {
	ZOOM_IN = 20,
	ZOOM_OUT = 90
};

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 target = glm::vec3(0.0f, 1.0f, 0.0f), float fov = 90.0f)
	{
		camPos = position;
		camTarget = target;
		currentFOV = fov;
		newFOV = fov;
	}

	//GET
	glm::vec3 getCamPos() { return camPos; }
	glm::vec3 getCamTarget() { return camTarget; }
	glm::vec3 getCamDistance() { return camDistance; }
	glm::vec3 getUpVector() { return upVector; }
	glm::vec3 getCamRight() { return camRight; }
	glm::vec3 getCamUp() { return camUp; }
	glm::mat4 getModel() { return model; }
	glm::mat4 getView() { return view; }
	glm::mat4 getProjection() { return projection; }
	float getCamSpeed() { return camSpeed; }
	float getCamSpeedFixed() { return camSpeedFixed; }
	float getCamCurrentFOV() { return currentFOV; }

	//SET
	void moveCam(camDirections Direction) {
		switch (Direction)
		{
		case FORWARD:
			camPos = camPos + (camSpeedFixed * camTarget);
			break;

		case BACKWARDS:
			camPos = camPos - (camSpeedFixed * camTarget);
			break;

		case LEFT:
			camPos = camPos - (glm::normalize(glm::cross(camTarget, upVector)) * camSpeedFixed);
			break;

		case RIGHT:
			camPos = camPos + (glm::normalize(glm::cross(camTarget, upVector)) * camSpeedFixed);
			break;

		default:
			LOG("ERROR::CAMERA::DEFAULT_INPUT");
			break;
		}
	}


	void setCamPos(glm::vec3 newCamPos) { camPos = newCamPos; UpdateValues(); }
	void setCamTarget(glm::vec3 newCamTarget) { camTarget = newCamTarget; UpdateValues(); }
	void setCamDistance(glm::vec3 newCamDistance) { camDistance = newCamDistance; UpdateValues(); }
	void setCamRight(glm::vec3 newCamRight) { camRight = newCamRight; UpdateValues(); }
	void setCamUp(glm::vec3 newCamUp) { camUp = newCamUp; UpdateValues(); }
	void setModel(glm::mat4 newModel) { model = newModel; UpdateValues(); }
	void setView(glm::mat4 newView) { view = newView; UpdateValues(); }
	void setProjection(glm::mat4 newProjection) { projection = newProjection; UpdateValues(); }
	void setFOV(int setFOV) { newFOV = setFOV; UpdateValues(); }
	void setSpeed(float speed) { camSpeed = speed; UpdateValues(); }

	//UPDATE
	void UpdateValues() {
		camDistance = glm::normalize(camPos - camTarget);
		camRight = glm::normalize(glm::cross(upVector, camDistance));
		camUp = glm::cross(camDistance, camRight);
		camSpeedFixed = camSpeed * deltaTime;

		if (!(newFOV == (int)currentFOV))
		{
			if (currentFOV > newFOV)
			{
				currentFOV -= (100 * deltaTime);
			}
			else
			{
				currentFOV += (100 * deltaTime);
			}
		}
		else {
			currentFOV = newFOV;
		}

		view = glm::lookAt(camPos, camTarget + camPos, upVector);
		projection = glm::perspective(glm::radians(currentFOV), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
	}

private:

	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDistance;
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 camRight;
	glm::vec3 camUp;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	float currentFOV;
	float newFOV;

	float camSpeedFixed;
	float camSpeed = 2.50f;
};
#endif