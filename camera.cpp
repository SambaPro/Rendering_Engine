#include "camera.h"

// Initial Camera Values
glm::vec3 Camera::INITIAL_CAM_POSITION = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 Camera::INITIAL_FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);
float Camera::SENSITIVITY = 0.05f;
float Camera::DEFAULT_SPEED = 3.0f;

// Camera Values
bool Camera::mouselock = true;
glm::vec3 Camera::positionVec = INITIAL_CAM_POSITION;
glm::vec3 Camera::directionVec = INITIAL_FRONT_VECTOR;
glm::vec3 Camera::upVec = glm::vec3(0.0f, 1.0f, 0.0f);
float Camera::pitch = 0.0f;
float Camera::yaw = -90.0f;
float Camera::speed = DEFAULT_SPEED;
float Camera::fov = 45.0f;
