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

	//SET
	void setCamPos(glm::vec3 newCamPos) { camPos = newCamPos; UpdateValues(); }
	void setCamTarget(glm::vec3 newCamTarget) { camTarget = newCamTarget; UpdateValues(); }
	void setCamDistance(glm::vec3 newCamDistance) { camDistance = newCamDistance; UpdateValues(); }
	void setCamRight(glm::vec3 newCamRight) { camRight = newCamRight; UpdateValues(); }
	void setCamUp(glm::vec3 newCamUp) { camUp = newCamUp; UpdateValues(); }
	void setModel(glm::mat4 newModel) { model = newModel; UpdateValues(); }
	void setView(glm::mat4 newView) { view = newView; UpdateValues(); }
	void setProjection(glm::mat4 newProjection) { projection = newProjection; UpdateValues(); }
	void setFOV(float setFOV) { newFOV = setFOV; UpdateValues(); }

	//UPDATE
	void UpdateValues() {
		camDistance = glm::normalize(camPos - camTarget);
		camRight = glm::normalize(glm::cross(upVector, camDistance));
		camUp = glm::cross(camDistance, camRight);
		camSpeed = 2.50f * deltaTime;

		if (!(currentFOV == newFOV))
		{
			if ((!(currentFOV == newFOV)) && currentFOV > newFOV)
			{
				currentFOV -= 5;
			}
			else
			{
				currentFOV += 5;
			}
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

	float camSpeed;
};
#endif