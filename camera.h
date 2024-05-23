#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


class Camera {
public:
	// Initial Values
	static glm::vec3 INITIAL_CAM_POSITION;
	static glm::vec3 INITIAL_FRONT_VECTOR;
	static float SENSITIVITY;
	static float DEFAULT_SPEED;

	// Camera Values
	static bool mouselock;
	static glm::vec3 positionVec;
	static glm::vec3 directionVec;
	static glm::vec3 upVec;
	static float pitch;
	static float yaw;
	static float speed;
	static float fov;
	

	static glm::mat4 getViewMatrix()
	{
		return glm::lookAt(positionVec, positionVec + directionVec, upVec);
	};


	static glm::mat4 getProjectionMatrix(float SCR_WIDTH, float SCR_HEIGHT)
	{
		return glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	};


	static void processMouse(float xoffset, float yoffset)
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

	static void printCamera()
	{
		std::cout << "Position Vec: " << Camera::positionVec.x << Camera::positionVec.y << Camera::positionVec.z << std::endl;
	};

private:
	Camera() {};

};

#endif