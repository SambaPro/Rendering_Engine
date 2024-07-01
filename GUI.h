#ifndef GUI_H
#define GUI_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

#include "resource_manager.h"
#include "settings.h"

class GUI
{
public:

    inline static bool show_demo_window = true;
    inline static bool show_another_window = false;
    inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static ImGuiIO& GUI_setup(GLFWwindow* window)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        return io;
    }

    static void GUI_loop(ImGuiIO& io)
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
            Settings::scale = 4.0;
            Settings::trans = glm::vec3(0.0f, 0.0f, 0.0f);
            ResourceManager::currentModel = ResourceManager::getModel("cubeModel");
        }

        ImGui::SameLine();

        if (ImGui::Button("Teapot"))
        {
            std::cout << "Changing model to Teapot" << std::endl;
            Settings::scale = 0.06f;
            Settings::trans = glm::vec3(0.0f, -2.0f, 0.0f);
            ResourceManager::currentModel = ResourceManager::getModel("teapotModel");
        }

        ImGui::SameLine();

        if (ImGui::Button("Sphere"))
        {
            std::cout << "Changing model to Sphere" << std::endl;
            Settings::scale = 1.0f;
            //Settings::trans = glm::vec3(0.0f, -2.0f, 0.0f);
            ResourceManager::currentModel = ResourceManager::getModel("sphereModel");
        }

        ImGui::SameLine();

        if (ImGui::Button("Cow"))
        {
            std::cout << "Changing model to Cow" << std::endl;
            Settings::scale = 0.8f;
            Settings::trans = glm::vec3(0.0f, 0.0f, 0.0f);
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
            ResourceManager::currentShader.setBool("blinn", false);
        }

        ImGui::SameLine();

        if (ImGui::Button("Blinn-Phong"))
        {
            std::cout << "Changing shader to Blinn-Phong" << std::endl;
            ResourceManager::currentShader = ResourceManager::getShader("phongShader");
            ResourceManager::currentShader.setBool("blinn", true);
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
};

#endif