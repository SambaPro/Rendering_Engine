#ifndef SETTINGS_H
#define SETTINGS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Settings
{
public:

    inline static const unsigned int SCREEN_WIDTH = 1024;
    inline static const unsigned int SCREEN_HEIGHT = 768;

    // Delta time
    inline static float deltaTime = 0.0f; // Time between current frame and last frame
    inline static float lastFrame = 0.0f; // Time of last frame

	inline static float scale = 4.0;
    inline static glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f);
    inline static bool texture_setting = false;;
    inline static bool blinn = false;

};

#endif