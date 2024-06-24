#include "camera.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Initial Camera Values
const glm::vec3 Camera::INITIAL_CAM_POSITION = glm::vec3(0.0f, 0.0f, 10.0f);
const glm::vec3 Camera::INITIAL_FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);
const float  Camera::SENSITIVITY = 0.05f;
const float Camera::DEFAULT_SPEED = 3.0f;

// Camera Values
bool Camera::mouselock = true;
glm::vec3 Camera::positionVec = INITIAL_CAM_POSITION;
glm::vec3 Camera::frontVec = INITIAL_FRONT_VECTOR;
glm::vec3 Camera::upVec = glm::vec3(0.0f, 1.0f, 0.0f);
float Camera::pitch = 0.0f;
float Camera::yaw = -90.0f;
float Camera::speed = DEFAULT_SPEED;
float Camera::fov = 45.0f;

// Function Definitions
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(positionVec, positionVec + frontVec, upVec);
};


glm::mat4 Camera::getProjectionMatrix(float SCR_WIDTH, float SCR_HEIGHT)
{
	return glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
};


void Camera::processMouse(float xoffset, float yoffset)
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