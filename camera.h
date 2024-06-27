#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


class Camera {
public:
	// Initial Values
	inline static const glm::vec3 INITIAL_CAM_POSITION = glm::vec3(0.0f, 0.0f, 10.0f);
	inline static const glm::vec3 INITIAL_FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);
	inline static const float SENSITIVITY = 0.05f;
	inline static const float DEFAULT_SPEED = 3.0f;

	// Camera Values
	inline static bool mouselock = true;
	inline static glm::vec3 positionVec = INITIAL_CAM_POSITION;
	inline static glm::vec3 frontVec = INITIAL_FRONT_VECTOR;
	inline static glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	inline static float pitch = 0.0f;
	inline static float yaw = -90.0f;
	inline static float speed = DEFAULT_SPEED;;
	inline static float fov = 45.0f;
	

	static glm::mat4 getViewMatrix() {return glm::lookAt(positionVec, positionVec + frontVec, upVec);}
	static glm::mat4 getProjectionMatrix(float SCR_WIDTH, float SCR_HEIGHT) {return glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);}
	
	static void processMouse(float xoffset, float yoffset)
	{
		xoffset *= SENSITIVITY;
		yoffset *= SENSITIVITY;

		yaw += xoffset;
		pitch += yoffset;

		// Prevent flipping when looking straight up or down
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		// update front vector
		glm::vec3 front{};
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontVec = glm::normalize(front);
	};
};

#endif