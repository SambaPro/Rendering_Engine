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


#include "camera.h"
#include "shader.h"
#include "model.h"
#include "resource_manager.h"


/* Declare functions */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void load_texture(const char* path, unsigned int textureID);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
glm::mat4 scale_world(glm::mat4 model, float factor);
void GUI_loop(ImGuiIO& io, bool show_demo_window, bool show_another_window, ImVec4 clear_color);

/* Settings */
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

/* Initial mouse settings */
bool initialMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

/* Delta time */
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{
    /* GLFW setup */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create glfw window */
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

    /* Enable depth rendering */
    glEnable(GL_DEPTH_TEST);

    /* ImGui setup */
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  

    /* Flip Textures */
    stbi_set_flip_vertically_on_load(true);

    /* Load Models */
    Model cubeModel("src/assets/cube/cube.obj");
    Model teapotModel("src/assets/teapot.obj");

    /* Load Shaders */
    Shader colShader("src/shaders/vertex_shader.vs", "src/shaders/colour_shader.fs");
    Shader texshader("src/shaders/vertex_shader.vs", "src/shaders/texture_shader.fs");
    ResourceManager::loadShader(colShader); // 0
    ResourceManager::loadShader(texshader); // 1
    Shader shader = ResourceManager::getShader(1);
    ResourceManager::currentShader = ResourceManager::getShader(1);

    std::cout << "Current shader ID is " << ResourceManager::currentShader.ID << std::endl;

    /* Load Texture */
    unsigned int texture1 {0};
    load_texture("src/assets/cube/default.png", texture1);
 

    /* Activate Shader and Texure */
    ResourceManager::currentShader.use();
    glUniform1i(glGetUniformLocation(ResourceManager::currentShader.ID, "texture1"), 0);


    /* loop  until user closes window */
    while (!glfwWindowShouldClose(window))
    {
        //shader = texShader;


        // delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /* Allow input */
        processInput(window);


        /* Clear Buffers */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        ResourceManager::currentShader.use();

        //get view and projection matrices and send to shader class
        glm::mat4 view = Camera::getViewMatrix();
        ResourceManager::currentShader.setMat4("view", view);

        glm::mat4 projection = Camera::getProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);
        ResourceManager::currentShader.setMat4("projection", projection);

        // render gemetry
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // centre model
        model = scale_world(model, 1);
        ResourceManager::currentShader.setMat4("model", model);
        cubeModel.drawModel(ResourceManager::currentShader);


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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forwards
        Camera::positionVec += Camera::speed * Camera::directionVec;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // back
        Camera::positionVec -= Camera::speed * Camera::directionVec;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // left
        Camera::positionVec -= glm::normalize(glm::cross(Camera::directionVec, Camera::upVec)) * Camera::speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
        Camera::positionVec += glm::normalize(glm::cross(Camera::directionVec, Camera::upVec)) * Camera::speed;

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

void load_texture(const char* path, unsigned int textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // flip image
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
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

    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Menu"); // create new window


    ImGui::Text("Change Model");
    if (ImGui::Button("Cube"))
    {
        std::cout << "Changing model to Cube" << std::endl;

    }

    ImGui::SameLine();

    if (ImGui::Button("Teapot"))
    {
        std::cout << "Changing model to Teapotd" << std::endl;

    }


    ImGui::Text("Change Shader");
    if (ImGui::Button("Texture"))
    {
        std::cout << "Changing shader to Texture" << std::endl;
        ResourceManager::currentShader = ResourceManager::getShader(1);
    }

    ImGui::SameLine();

    if (ImGui::Button("Colour"))
    {
        std::cout << "Changing shader to Colour" << std::endl;
        ResourceManager::currentShader = ResourceManager::getShader(0);
    } 


    /*
    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    */

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}
