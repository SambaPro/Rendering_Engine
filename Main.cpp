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

#include "resource_manager.h"
#include "camera.h"
#include "shader.h"
#include "model.h"
#include "texture.h"


// Declare functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void load_models();
void load_shaders();
void load_textures();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
glm::mat4 scale_world(glm::mat4 model, float factor);
void GUI_loop(ImGuiIO& io, bool show_demo_window, bool show_another_window, ImVec4 clear_color);

// Settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
float scale = 4.0;
glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f);
bool texture_setting = false;

// Initial mouse settings
bool initialMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

// Delta time
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{
    // GLFW setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create glfw window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphical Engine", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Sets callback function which is called when window is resized
    glfwSetCursorPosCallback(window, mouse_callback);                  // Sets callback function which is called when mouse is moved
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);       // Locks and centres mouse to window


    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // Enable depth rendering
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);

    // ImGui setup
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  
    // Flip Textures
    stbi_set_flip_vertically_on_load(true);

    // Load Resources
    //ResourceManager::initialise();
    load_models();
    load_shaders();
    load_textures();



    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);


        // Clear Buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        ResourceManager::currentShader.use();

        // get view and projection matrices and send to shader class
        glm::mat4 view = Camera::getViewMatrix();
        ResourceManager::currentShader.setMat4("view", view);

        glm::mat4 projection = Camera::getProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);
        ResourceManager::currentShader.setMat4("projection", projection);

        // render gemetry
        glm::mat4 model = glm::mat4(1.0f); // identity matrix
        model = glm::translate(model, trans); // translate model
        model = scale_world(model, scale);    // scale model
        ResourceManager::currentShader.setMat4("model", model);

        ResourceManager::currentShader.setVec3("viewPos", Camera::positionVec);


        // Change light position
        double time = glfwGetTime();
        ResourceManager::currentShader.setVec3("light.pos", glm::vec3(10 * glm::cos(time), 10 * glm::sin(time), 0.0f));

        // Draw model
        ResourceManager::currentModel.drawModel();



        /* ImuGUI */
        GUI_loop(io, show_demo_window, show_another_window, clear_color);


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

/* Callback function to adapt window size when resizing window */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* Processes player keyboard input */
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Camera::speed = Camera::DEFAULT_SPEED * deltaTime;

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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

/* Callback function called when a mouse button is pressed */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
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

void load_models()
{
    Model cubeModel("src/assets/cube/cube.obj");
    Model teapotModel("src/assets/teapot.obj");
    Model carModel("src/assets/bmw/bmw.obj");
    Model cowModel("src/assets/cow.obj");
    ResourceManager::loadModel("cubeModel", cubeModel);
    ResourceManager::loadModel("teapotModel", teapotModel);
    ResourceManager::loadModel("carModel", carModel);
    ResourceManager::loadModel("cowModel", cowModel);

    ResourceManager::currentModel = ResourceManager::getModel("cubeModel");
}

void load_shaders()
{
    Shader phongShader("src/shaders/vertex_shader.vs", "src/shaders/phong_shader.fs");
    Shader defaultShader("src/shaders/vertex_shader.vs", "src/shaders/default_shader.fs");
    ResourceManager::loadShader("phongShader", phongShader);
    ResourceManager::loadShader("defaultShader", defaultShader);

    //TODO UNIFORM BUFFER

    // load phong shader
    ResourceManager::currentShader = ResourceManager::getShader("phongShader");
    ResourceManager::currentShader.use();
    ResourceManager::currentShader.setVec3("objectColour", glm::vec3(1.0f, 0.5f, 0.0f));

    // load light source
    ResourceManager::currentShader.setVec3("light.pos", glm::vec3(100.0f, 100.0f, 100.0f)); // Position vector of light source
    ResourceManager::currentShader.setVec3("light.colour", glm::vec3(1.0f, 1.0f, 1.0f));    // Colour of ambient light
    ResourceManager::currentShader.setFloat("light.ambientAlbedo", 0.5f);                   // Intensity of ambient light
    ResourceManager::currentShader.setFloat("light.diffuseAlbedo", 0.5f);                   // Intensity of diffuse light
    ResourceManager::currentShader.setFloat("light.specularAlbedo", 0.5f);                  // Intensity of specular light

    // load default shader
    ResourceManager::currentShader = ResourceManager::getShader("defaultShader");
    ResourceManager::currentShader.use();
    ResourceManager::currentShader.setVec3("objectColour", glm::vec3(1.0f, 0.5f, 0.0f));


}

void load_textures()
{
    Texture cube_default_texture = Texture("src/assets/cube/default.png");
    ResourceManager::loadTexture("default", cube_default_texture);

    Texture brick_texture = Texture("src/assets/brick.jpg");
    ResourceManager::loadTexture("brick", brick_texture);

    ResourceManager::currentTexture = ResourceManager::getTexture("default");
    glBindTexture(GL_TEXTURE_2D, ResourceManager::currentTexture.ID);
}


glm::mat4 scale_world(glm::mat4 model, float factor)
{
    model = glm::scale(model, glm::vec3(factor, factor, factor));

    return model;
}

void GUI_loop(ImGuiIO& io, bool show_demo_window, bool show_another_window, ImVec4 clear_color)
{
    // ImGui Setup New Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Menu"); // create new window


    ImGui::Text("Change Model");
    if (ImGui::Button("Cube"))
    {
        std::cout << "Changing model to Cube" << std::endl;
        scale = 4.0;
        trans = glm::vec3(0.0f, 0.0f, 0.0f);
        ResourceManager::currentModel = ResourceManager::getModel("cubeModel");
    }

    ImGui::SameLine();

    if (ImGui::Button("Teapot"))
    {
        std::cout << "Changing model to Teapot" << std::endl;
        scale = 0.06f;
        trans = glm::vec3(0.0f, -2.0f, 0.0f);
        ResourceManager::currentModel = ResourceManager::getModel("teapotModel");
    }

    ImGui::SameLine();

    if (ImGui::Button("Car"))
    {
        std::cout << "Changing model to Car" << std::endl;
        scale = 0.06f;
        trans = glm::vec3(0.0f, 0.0f, 0.0f);
        ResourceManager::currentModel = ResourceManager::getModel("carModel");
    }

    ImGui::SameLine();

    if (ImGui::Button("Cow"))
    {
        std::cout << "Changing model to Cow" << std::endl;
        scale = 0.06f;
        trans = glm::vec3(0.0f, 0.0f, 0.0f);
        ResourceManager::currentModel = ResourceManager::getModel("cowModel");
    }


    ImGui::Text("Change Shader");
    if (ImGui::Button("Default"))
    {
        std::cout << "Changing shader to Texture" << std::endl;
        ResourceManager::currentShader = ResourceManager::getShader("defaultShader");
    }

    ImGui::SameLine();

    if (ImGui::Button("Phong"))
    {
        std::cout << "Changing shader to Phong" << std::endl;
        ResourceManager::currentShader = ResourceManager::getShader("phongShader");
    } 


    ImGui::Text("Change Texture");

    if (ImGui::Button("Colour"))
    {
        std::cout << "Changing T=texture to Colour" << std::endl;
        ResourceManager::currentShader.setBool("texture_setting", false);
        
    } 
    ImGui::SameLine();

    if (ImGui::Button("Default Texture"))
    {
        std::cout << "Changing texture to default" << std::endl;
        ResourceManager::currentShader.setBool("texture_setting", true);
        ResourceManager::currentTexture = ResourceManager::getTexture("default");
        glBindTexture(GL_TEXTURE_2D, ResourceManager::currentTexture.ID);
    } 
    ImGui::SameLine();

    if (ImGui::Button("Brick"))
    {
        std::cout << "Changing texture to Brick" << std::endl;
        ResourceManager::currentShader.setBool("texture_setting", true);
        ResourceManager::currentTexture = ResourceManager::getTexture("brick");
        glBindTexture(GL_TEXTURE_2D, ResourceManager::currentTexture.ID);
    }




    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}
