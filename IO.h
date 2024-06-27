#ifndef IO_H
#define IO_H

#include <GLFW/glfw3.h>

#include "settings.h"
#include "camera.h"

class IO
{
public:
	// Initial mouse settings
	inline static bool initialMouse = true;
	inline static float lastX = Settings::SCREEN_WIDTH / 2.0f;
	inline static float lastY = Settings::SCREEN_HEIGHT / 2.0f;

    // Processes player keyboard input
    static void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        Camera::speed = Camera::DEFAULT_SPEED * Settings::deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // double movement speed while left shift is held down
            Camera::speed = Camera::speed * 2;

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // Reset camera position vector
        {
            Camera::positionVec = Camera::INITIAL_CAM_POSITION;
            Camera::frontVec = Camera::INITIAL_FRONT_VECTOR;
            Camera::pitch = 0.0f;
            Camera::yaw = -90.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forwards
            Camera::positionVec += Camera::speed * Camera::frontVec;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // back
            Camera::positionVec -= Camera::speed * Camera::frontVec;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
            Camera::positionVec -= glm::normalize(glm::cross(Camera::frontVec, Camera::upVec)) * Camera::speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
            Camera::positionVec += glm::normalize(glm::cross(Camera::frontVec, Camera::upVec)) * Camera::speed;

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // up
            Camera::positionVec += Camera::upVec * Camera::speed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // down
            Camera::positionVec -= Camera::upVec * Camera::speed;
    }

    /* Callback function called when mouse is moved */
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (!Camera::mouselock) // prevents movement
        {
            lastX = xpos;
            lastY = ypos;
            return;
        }

        if (initialMouse)
        {
            lastX = xpos;
            lastY = ypos;
            initialMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;

        Camera::processMouse(xoffset, yoffset);
    }

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            if (Camera::mouselock == true)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                Camera::mouselock = false;
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                Camera::mouselock = true;
            }
    }
};

#endif