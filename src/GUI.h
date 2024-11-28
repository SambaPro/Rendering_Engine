#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <string>

#include "resource_manager.h"
#include "settings.h"

class GUI
{
public:

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

        //------------- Switch Models -------------------
        if (ImGui::CollapsingHeader("Change Model"))
        {
            if (ImGui::Button("Cube"))
            {
                std::cout << "Changing model to Cube" << std::endl;
                    ResourceManager::currentModel = ResourceManager::getModel("cubeModel");
            }

            ImGui::SameLine();

            if (ImGui::Button("Teapot"))
            {
                std::cout << "Changing model to Teapot" << std::endl;
                ResourceManager::currentModel = ResourceManager::getModel("teapotModel");
            }

            ImGui::SameLine();

            if (ImGui::Button("Sphere"))
            {
                std::cout << "Changing model to Sphere" << std::endl;
                ResourceManager::currentModel = ResourceManager::getModel("sphereModel");
            }

            ImGui::SameLine();

            if (ImGui::Button("Cow"))
            {
                std::cout << "Changing model to Cow" << std::endl;
                ResourceManager::currentModel = ResourceManager::getModel("cowModel");
            }
        };


        //------------- Switch Shaders -------------------
        if (ImGui::CollapsingHeader("Change Shader"))
        {
            if (ImGui::Button("Default"))
            {
                std::cout << "Changing shader to Default" << std::endl;
                ResourceManager::currentShader = ResourceManager::getShader("defaultShader");
            }

            ImGui::SameLine();

            if (ImGui::Button("Phong"))
            {
                std::cout << "Changing shader to Phong" << std::endl;
                ResourceManager::currentShader = ResourceManager::getShader("phongShader");
                Settings::blinn = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Blinn-Phong"))
            {
                std::cout << "Changing shader to Blinn-Phong" << std::endl;
                ResourceManager::currentShader = ResourceManager::getShader("phongShader");
                Settings::blinn = true;
            }

            ImGui::SameLine();

            if (ImGui::Button("Ray Tracer"))
            {
                std::cout << "Changing shader to Ray Tracer" << std::endl;
                ResourceManager::currentShader = ResourceManager::getShader("raytracerShader");
            }
        }

        //------------- Texture Options-------------------
        if (ImGui::CollapsingHeader("Change Texture"))
        {
            if (ImGui::Button("None"))
            {
                std::cout << "Changing to None" << std::endl;
                Settings::texture_setting = false;

            }
            ImGui::SameLine();

            if (ImGui::Button("Default Texture"))
            {
                std::cout << "Changing texture to default" << std::endl;
                Settings::texture_setting = true;
                ResourceManager::currentTexture = ResourceManager::getTexture("default");
                glBindTexture(GL_TEXTURE_2D, ResourceManager::currentTexture.ID);
            }
            ImGui::SameLine();

            if (ImGui::Button("Brick"))
            {
                std::cout << "Changing texture to Brick" << std::endl;
                Settings::texture_setting = true;
                ResourceManager::currentTexture = ResourceManager::getTexture("brick");
                glBindTexture(GL_TEXTURE_2D, ResourceManager::currentTexture.ID);
            }
        }

        //------------- Material Options-------------------
        if (ImGui::CollapsingHeader("Change Material"))
        {
            static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
            Material& material = ResourceManager::currentMaterial;

            ImGui::SliderFloat("Ambient Albedo", &material.ambientAlbedo, 0.0f, 1.0f, "%.3f", flags);
            ImGui::SliderFloat("Diffuse Albedo", &material.diffuseAlbedo, 0.0f, 1.0f, "%.3f", flags);
            ImGui::SliderFloat("Specular Albedo", &material.specularAlbedo, 0.0f, 1.0f, "%.3f", flags);
            ImGui::SliderFloat("Shininess", &material.shininess, 1.0f, 256.0f, "%.3f", flags);
           
            float colour[3] = { material.colour.x, material.colour.y, material.colour.z };
            ImGui::ColorEdit3("colour", colour);
            material.colour = glm::vec3(colour[0], colour[1], colour[2]);


        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();


        ImGui::Begin("Lights"); // create new window

        //------------- Light Options-------------------
        ImGui::Text("Light Settings");
        for (int i = 0; i < size(ResourceManager::LightSources); ++i)
        {
            LightSource& light = ResourceManager::LightSources[i];
            std::string name = "Light " + std::to_string(i);

            if (ImGui::CollapsingHeader(name.c_str()))
            {
                ImGui::PushID(i);
                static ImGuiSliderFlags flags = ImGuiSliderFlags_None;

                // Checkboxes
                ImGui::Checkbox("Active", &light.activated);
                ImGui::Checkbox("Pointlight", &light.pointLight);

                // Orbit
                ImGui::Checkbox("Orbit", &light.orbit);
                ImGui::SliderFloat("Radius", &light.radius, 0.0f, 10.0f, "%.3f", flags);
                ImGui::SliderFloat("Orbit Speed", &light.orbit_speed, 0.0f, 10.0f, "%.3f", flags);

                // Light Position
                ImGui::Text("Light Position");
                ImGui::SliderFloat("X Position", &light.posVec.x, -10.0f, 10.0f, "%.3f", flags);
                ImGui::SliderFloat("Y Position", &light.posVec.y, -10.0f, 10.0f, "%.3f", flags);
                ImGui::SliderFloat("Z Position", &light.posVec.z, -10.0f, 10.0f, "%.3f", flags);

                // Light Colour
                float colour[3] = { light.colour.x, light.colour.y, light.colour.z };
                ImGui::ColorEdit3("colour", colour);
                light.colour = glm::vec3(colour[0], colour[1], colour[2]);

                ImGui::PopID();
            }
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
};