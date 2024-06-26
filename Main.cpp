#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


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


    // glad: load all OpenGL function pointers
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

    // light source
    //LightSource Light(ResourceManager::getShader("lightShader"));


    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        Settings::deltaTime = currentFrame - Settings::lastFrame;
        Settings::lastFrame = currentFrame;

        IO::processInput(window);


        // Clear Buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ResourceManager::currentShader.use();

        // get view and projection matrices and send to shader class
        glm::mat4 view = Camera::getViewMatrix();
        ResourceManager::currentShader.setMat4("view", view);

        glm::mat4 projection = Camera::getProjectionMatrix(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT);
        ResourceManager::currentShader.setMat4("projection", projection);

        // render gemetry
        glm::mat4 model = glm::mat4(1.0f); // identity matrix
        model = glm::translate(model, Settings::trans); // translate model
        //model = glm::translate(model, glm::vec3(100.0f)); // TEMP
        model = glm::scale(model, glm::vec3(Settings::scale)); // scale model
        ResourceManager::currentShader.setMat4("model", model);

        ResourceManager::currentShader.setVec3("viewPos", Camera::positionVec);


        // ---------------------------------------------------------------
        // Change light position
        float time = static_cast<float>(glfwGetTime());
        LightSource& Light = ResourceManager::LightSources[0];

        Light.posVec.x = 10 * glm::sin(time);
        Light.posVec.y = 5 * glm::sin(time);
        Light.posVec.z = 10 * glm::cos(time);
 
        //Light.posVec = glm::vec3(0.0f, 0.0f, -10.0f);
        ResourceManager::currentShader.setVec3("light.pos", Light.posVec);

        Light.drawLight(ResourceManager::getShader("lightShader"), projection, view);
        
        //------------------------------------------------------------------

        // Draw model
        ResourceManager::currentModel.drawModel(ResourceManager::currentShader);


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

