#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include <map>

#include "texture.h"
#include "shader.h"
#include "model.h"

class ResourceManager
{
public:
	inline static std::map<std::string, Shader> Shaders;
	inline static std::map<std::string, Texture> Textures;
	inline static std::map<std::string, Model> Models;

	inline static Shader currentShader;
	inline static Model currentModel;
	inline static Texture currentTexture;

	static void initialise()
	{
		initialiseShaders();
		initialiseModels();
		initialiseTextures();
	};

	// Getters
	static Shader getShader(std::string name) {return ResourceManager::Shaders[name];}
	static Model getModel(std::string name) {return ResourceManager::Models[name];}
	static Texture getTexture(std::string name) {return ResourceManager::Textures[name];}

	// Load resource into manager
	static void loadShader(std::string name, Shader shader)
	{
		ResourceManager::Shaders[name] = shader;
		std::cout << "Shader ID: " << ResourceManager::Shaders[name].ID << " loaded successfully" << std::endl;
	};

	static void loadModel(std::string name, Model model)
	{
		ResourceManager::Models[name] = model;
		std::cout << "Model loaded successfully" << std::endl;
	};

	static void loadTexture(std::string name, Texture texture)
	{
		ResourceManager::Textures[name] = texture;
		std::cout << "Texture ID: " << ResourceManager::Textures[name].ID << " loaded successfully" << std::endl;
	};

private:
	static void initialiseShaders()
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
		ResourceManager::currentShader.setFloat("shininess", 64.0);

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
	};

	static void initialiseModels()
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

	static void initialiseTextures()
	{	
		Texture cube_default_texture = Texture("src/assets/cube/default.png");
		ResourceManager::loadTexture("default", cube_default_texture);

		Texture brick_texture = Texture("src/assets/brick.jpg");
		ResourceManager::loadTexture("brick", brick_texture);

		ResourceManager::currentTexture = ResourceManager::getTexture("default");
		glBindTexture(GL_TEXTURE_2D, ResourceManager::currentTexture.ID);
	}

};

#endif

