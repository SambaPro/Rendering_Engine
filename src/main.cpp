#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "settings.h"
#include "resource_manager.h"
#include "IO.h"
#include "GUI.h"
#include "camera.h"
#include "shader.h"
#include "model.h"
#include "texture.h"
#include "light_source.h"


// Declare functions
void framebuffer_size_callback(GLFWwindow * window, int width, int height);


int main()
{
    // GLFW setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create glfw window
    GLFWwindow* window = glfwCreateWindow(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT, "Graphical Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Sets callback function which is called when window is resized
    glfwSetCursorPosCallback(window, IO::mouse_callback);              // Sets callback function which is called when mouse is moved
    glfwSetMouseButtonCallback(window, IO::mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);        // Locks and centres mouse to window


    //glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // Enable depth rendering
    glEnable(GL_DEPTH_TEST);

    // ImGui setup
    ImGuiIO& io = GUI::GUI_setup(window);

    // Flip Textures
    stbi_set_flip_vertically_on_load(true);

    // Load Resources
    ResourceManager::initialise();


    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        Settings::deltaTime = currentFrame - Settings::lastFrame;
        Settings::lastFrame = currentFrame;

        IO::processInput(window);


        // Clear Buffers
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ResourceManager::currentShader.use();

        for (auto& light : ResourceManager::LightSources)
        {
            light.processLight();
        }


        // Update shader data
        ResourceManager::uploadDatatoShader();

        // Draw commands
        ResourceManager::currentModel.drawModel(ResourceManager::currentShader);
        ResourceManager::drawLights(ResourceManager::getShader("lightShader"),
            Camera::getProjectionMatrix(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT),
            Camera::getViewMatrix());

        // Render GUI
        GUI::GUI_loop(io);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

// Callback function to adapt window size when resizing window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

