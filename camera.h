#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

// default camera settings
const glm::vec3 INITIAL_CAM_POSITION = glm::vec3(0.0f, 0.0f, 5.0f);
const glm::vec3 INITIAL_FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);
const float SENSITIVITY = 0.05f;
const float DEFAULT_SPEED = 3.0f;

class Camera {
public:
	bool mouselock;
	glm::vec3 positionVec; // initial camera position
	glm::vec3 directionVec;    // camera direction
	glm::vec3 upVec;       // up vector
	float pitch;
	float yaw;
	float speed;
	float fov;

	/* Constructor */
	Camera(glm::vec3 positionvec = INITIAL_CAM_POSITION, glm::vec3 directionvec = INITIAL_FRONT_VECTOR, glm::vec3 upvec = glm::vec3(0.0f, 1.0f, 0.0f), float Pitch = 0.0f, float Yaw = -90.0f, float Speed = DEFAULT_SPEED, float Fov = 45.0f)
	{
		mouselock = true;
		positionVec = positionvec;
		directionVec = directionvec;
		upVec = upvec;
		pitch = Pitch;
		yaw = Yaw;
		speed = Speed;
		fov = Fov;
	};


	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(positionVec, positionVec + directionVec, upVec);
	};


	glm::mat4 getProjectionMatrix(float SCR_WIDTH, float SCR_HEIGHT)
	{
		return glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	};


	void processMouse(float xoffset, float yoffset)
	{
		//std::cout << "Processing Mouse" << std::endl;
		xoffset *= SENSITIVITY;
		yoffset *= SENSITIVITY;

		yaw += xoffset;
		pitch += yoffset;

		// prevent flipping when looking striaght up or down
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		// update front vector
		glm::vec3 front{};
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		directionVec = glm::normalize(front);
	};
};

#endif