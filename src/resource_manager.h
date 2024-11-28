#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include <map>

#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "model.h"
#include "light_source.h"
#include "material.h"

class ResourceManager
{
public:
	inline static std::map<std::string, Shader> Shaders;
	inline static std::map<std::string, Texture> Textures;
	inline static std::map<std::string, Model> Models;
	inline static std::vector<LightSource> LightSources;

	inline static Shader currentShader;
	inline static Model currentModel;
	inline static Texture currentTexture;
	inline static Material currentMaterial;

	static void initialise()
	{
		initialiseShaders();
		initialiseModels();
		initialiseTextures();
		initialiseLightSources();
		initialiseMaterials();
		currentShader = getShader("defaultShader");
		currentShader.use();
	};

	// Getters
	static Shader getShader(std::string name) {return Shaders[name];}
	static Model getModel(std::string name) {return Models[name];}
	static Texture getTexture(std::string name) {return Textures[name];}

	// Load resource into manager
	static void loadShader(std::string name, Shader shader)
	{
		Shaders[name] = shader;
		std::cout << "Shader ID: " << Shaders[name].ID << " loaded successfully" << std::endl;
	}

	static void loadModel(std::string name, Model model)
	{
		Models[name] = model;
		std::cout << "Model loaded successfully" << std::endl;
	}

	static void loadTexture(std::string name, Texture texture)
	{
		Textures[name] = texture;
		std::cout << "Texture ID: " << Textures[name].ID << " loaded successfully" << std::endl;
	}

	static void uploadDatatoShader()
	{
		currentShader.use();

		// Settings
		currentShader.setBool("settings.texture_setting", Settings::texture_setting);
		currentShader.setBool("settings.blinn", Settings::blinn);

		// Transform Matrices
		currentShader.setMat4("model", currentModel.modelMatrix);
		ResourceManager::currentShader.setMat4("view", Camera::getViewMatrix());
		ResourceManager::currentShader.setMat4("projection", Camera::getProjectionMatrix(Settings::SCREEN_WIDTH, Settings::SCREEN_HEIGHT));

		// Material Data
		Material& material = ResourceManager::currentMaterial;
		currentShader.setVec3("material.colour", material.colour);
		currentShader.setFloat("material.ambientAlbedo", material.ambientAlbedo);
		currentShader.setFloat("material.diffuseAlbedo", material.diffuseAlbedo);
		currentShader.setFloat("material.specularAlbedo", material.specularAlbedo);
		currentShader.setFloat("material.shininess", material.shininess);

		// Camera Data
		currentShader.setVec3("viewPos", Camera::positionVec);

		// Light Data
		LightSource& light = LightSources[0];
		currentShader.setVec3("light[0].posVec", light.posVec);
		currentShader.setVec3("light[0].colour", light.colour);
		currentShader.setBool("light[0].pointLight", light.pointLight);
		currentShader.setBool("light[0].activated", light.activated);
		currentShader.setBool("light[0].orbit", light.orbit);
		currentShader.setFloat("light[0].radius", light.radius);
		
		LightSource& light2 = LightSources[1];
		currentShader.setVec3("light[1].posVec", light2.posVec);
		currentShader.setVec3("light[1].colour", light2.colour);
		currentShader.setBool("light[1].pointLight", light2.pointLight);
		currentShader.setBool("light[1].activated", light2.activated);
		currentShader.setBool("light[1].orbit", light2.orbit);
		currentShader.setFloat("light[1].radius", light2.radius);
		
	}

	static void drawLights(Shader shader, glm::mat4 projection, glm::mat4 view)
	{
		for (auto light : LightSources)
		{
			light.drawLight(shader, projection, view);
		}
	}

private:
	static void initialiseShaders()
	{
		Shader phongShader("../resources/shaders/vertex_shader.vs", "../resources/shaders/phong_shader.fs");
		Shader defaultShader("../resources/shaders/vertex_shader.vs", "../resources/shaders/default_shader.fs");
		Shader lightShader("../resources/shaders/lightsource_shader.vs", "../resources/shaders/lightsource_shader.fs");
		loadShader("phongShader", phongShader);
		loadShader("defaultShader", defaultShader);
		loadShader("lightShader", lightShader);

	};

	static void initialiseModels()
	{
		Model cubeModel("../resources/assets/cube/cube.obj");
		cubeModel.modelMatrix = glm::scale(cubeModel.modelMatrix, glm::vec3(4.0f));
		cubeModel.modelMatrix = glm::translate(cubeModel.modelMatrix, glm::vec3(0.0f));

		Model teapotModel("../resources/assets/teapot.obj");
		teapotModel.modelMatrix = glm::scale(teapotModel.modelMatrix, glm::vec3(0.06f));
		teapotModel.modelMatrix = glm::translate(teapotModel.modelMatrix, glm::vec3(0.0f, -30.0f, 0.0f));

		Model sphereModel("../resources/assets/sphere.obj");
		sphereModel.modelMatrix = glm::scale(sphereModel.modelMatrix, glm::vec3(1.0f));
		sphereModel.modelMatrix = glm::translate(sphereModel.modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

		Model cowModel("../resources/assets/cow.obj");
		sphereModel.modelMatrix = glm::scale(sphereModel.modelMatrix, glm::vec3(0.8f));
		sphereModel.modelMatrix = glm::translate(sphereModel.modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

		loadModel("cubeModel", cubeModel);
		loadModel("teapotModel", teapotModel);
		loadModel("sphereModel", sphereModel);
		loadModel("cowModel", cowModel);

		currentModel = getModel("cubeModel");
	}

	static void initialiseTextures()
	{	
		Texture cube_default_texture = Texture("../resources/assets/cube/default.png");
		loadTexture("default", cube_default_texture);

		Texture brick_texture = Texture("../resources/assets/brick.jpg");
		loadTexture("brick", brick_texture);

		currentTexture = getTexture("default");
		glBindTexture(GL_TEXTURE_2D, currentTexture.ID);
	}

	static void initialiseLightSources()
	{
		LightSource light1(getShader("lightShader"));
		light1.activated = true;
		light1.posVec = glm::vec3(0.0f, 0.0f, 10.0f);
		LightSources.push_back(light1);
		
		LightSource light2(getShader("lightShader"));
		light2.activated = false;
		light2.posVec = glm::vec3(10.0f);
		LightSources.push_back(light2);
	}

	static void initialiseMaterials()
	{
		Material custom;
		custom.colour = glm::vec3(1.0f, 0.5f, 0.0f);
		custom.shininess = 128.0f;
		custom.ambientAlbedo = 0.5f;
		custom.diffuseAlbedo = 0.5f;
		custom.specularAlbedo = 0.5f;

		currentMaterial = custom;
	}
};

#endif

