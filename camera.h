#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


class Camera {
public:
	// Initial Values
	static const glm::vec3 INITIAL_CAM_POSITION;
	static const glm::vec3 INITIAL_FRONT_VECTOR;
	static const float SENSITIVITY;
	static const float DEFAULT_SPEED;

	// Camera Values
	static bool mouselock;
	static glm::vec3 positionVec;
	static glm::vec3 frontVec;
	static glm::vec3 upVec;
	static float pitch;
	static float yaw;
	static float speed;
	static float fov;
	

	static glm::mat4 getViewMatrix();
	static glm::mat4 getProjectionMatrix(float SCR_WIDTH, float SCR_HEIGHT);
	static void processMouse(float xoffset, float yoffset);

};

#endif