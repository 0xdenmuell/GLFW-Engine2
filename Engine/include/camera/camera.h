#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>

extern float SCR_WIDTH;
extern float SCR_HEIGHT;

class Camera
{
public:
	Camera() {
		camPos =		glm::vec3(0.0f, 0.0f, 3.0f);
		camTarget =		glm::vec3(0.0f, 0.0f, -1.0f);
		camDistance =	glm::normalize(camPos - camTarget);
		upVector =		glm::vec3(0.0f, 1.0f, 0.0f);
		camRight =		glm::normalize(glm::cross(upVector, camDistance));
		camUp =			glm::cross(camDistance, camRight);

		model =			glm::mat4(1.0f);
		view =			glm::mat4(1.0f);
		projection =	glm::mat4(1.0f);

		view =			glm::lookAt(camPos, camTarget, upVector);
		projection =	glm::perspective(glm::radians(90.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
	}

	//GET
	float getCamSpeed() { return camSpeed; }
	glm::vec3 getCamPos() { return camPos; }
	glm::vec3 getCamTarget() { return camTarget; }
	glm::vec3 getCamDistance() { return camDistance; }
	glm::vec3 getUpVector() { return upVector; }
	glm::vec3 getCamRight() { return camRight; }
	glm::vec3 getCamUp() { return camUp; }
	glm::mat4 getModel() { return model; }
	glm::mat4 getView() { 
		view = glm::lookAt(camPos, camTarget + camPos, upVector);
		return view; 
	}
	glm::mat4 getProjection() { return projection; }

	//SET
	void setCamPos(glm::vec3 newCamPos) { camPos = newCamPos; }
	void setCamTarget(glm::vec3 newCamTarget) { camTarget = newCamTarget; }
	void setCamDistance(glm::vec3 newCamDistance) { camDistance = newCamDistance; }
	void setCamRight(glm::vec3 newCamRight) { camRight = newCamRight; }
	void setCamUp(glm::vec3 newCamUp) { camUp = newCamUp; }
	void setModel(glm::mat4 newModel) { model = newModel; }
	void setView(glm::mat4 newView) { view = newView; }
	void setProjection(glm::mat4 newProjection) { projection = newProjection; }

private:

	glm::vec3 camPos;
	glm::vec3 camTarget;
	glm::vec3 camDistance;
	glm::vec3 upVector;
	glm::vec3 camRight;
	glm::vec3 camUp;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	const float camSpeed = 0.05f;
};
#endif
