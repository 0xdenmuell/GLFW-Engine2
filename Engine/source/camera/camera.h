#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <window/window.h>

#include <iostream>

extern float g_SCR_WIDTH;
extern float g_SCR_HEIGHT;
extern float deltaTime;

#define LOG(msg) std::cout << msg << std::endl
#define vec3LOG(vec3) std::cout << vec3.x << "|" << vec3.y << "|" << vec3.z << std::endl
#define vec4LOG(vec3) std::cout << vec3.x << "|" << vec3.y << "|" << vec3.z << "|" << vec3.w << std::endl

enum cameraDirections {
	FORWARD = 1,
	BACKWARDS = 2,
	LEFT = 3,
	RIGHT = 4
};
enum cameraZoom {
	ZOOM_IN = 20,
	ZOOM_OUT = 90
};
enum cameraSpeed {
	LOW = 3, HIGH = 10
};

class Camera
{
public:
	glm::vec3 m_position;
	glm::vec3 m_target;
	float m_speed = 2.50f;
	float m_fov = 90.0f;

	Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 target = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		m_position = position;
		m_target = target;
	}

	//GET
	glm::mat4 getModel() { return model; }
	glm::mat4 getView() { return view; }
	glm::mat4 getProjection() { return projection; }

	//SET
	void setModel(glm::mat4 newModel) { model = newModel; UpdateValues(); }
	void setView(glm::mat4 newView) { view = newView; UpdateValues(); }
	void setProjection(glm::mat4 newProjection) { projection = newProjection; UpdateValues(); }

	//MOVEMENT
	void setMovementPosition(cameraDirections Direction) {
		switch (Direction)
		{
		case FORWARD:
			m_position = m_position + (speedFixed * m_target);
			break;
		case BACKWARDS:
			m_position = m_position - (speedFixed * m_target);
			break;
		case LEFT:
			m_position = m_position - (glm::normalize(glm::cross(m_target, upNormal)) * speedFixed);
			break;
		case RIGHT:
			m_position = m_position + (glm::normalize(glm::cross(m_target, upNormal)) * speedFixed);
			break;
		default:
			LOG("ERROR::CAMERA::DEFAULT_INPUT");
			break;
		}
	}
	void setMovementTarget(double posX, double posY) {

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
		m_target = glm::normalize(direction);
	}

	//UPDATE
	void UpdateValues() {
		distance = glm::normalize(m_position - m_target);
		rightVector = glm::normalize(glm::cross(upNormal, distance));
		upVector = glm::cross(distance, rightVector);
		speedFixed = m_speed * deltaTime;

		/*
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
		*/

		view = glm::lookAt(m_position, m_target + m_position, upNormal);
		projection = glm::perspective(glm::radians(m_fov), g_SCR_WIDTH / g_SCR_HEIGHT, 0.1f, 100.0f);
	}


private:
	glm::vec3 distance;
	glm::vec3 upVector;
	glm::vec3 rightVector;

	glm::vec3 upNormal = glm::vec3(0.0f, 1.0f, 0.0f);
	float speedFixed;

	//MovementTarget
	float lastX = 400;
	float lastY = 300;
	float yaw = 90.0f;
	float pitch = 0;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

};
#endif