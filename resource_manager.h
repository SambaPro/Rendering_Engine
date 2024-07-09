#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include <map>

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
	inline static std::map<std::string, Material> Materials;

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
	static Material getMaterial(std::string name) {return Materials[name];}

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

	static void loadMaterial(std::string name, Material material)
	{
		Materials[name] = material;
		std::cout << "Material " << name << " loaded successfully" << std::endl;
	}

	static void uploadDatatoShader()
	{
		currentShader.use();

		// TODO UNIFORM BUFFER OBJECT

		// Settings
		currentShader.setBool("settings.texture_setting", Settings::texture_setting);
		currentShader.setBool("settings.blinn", Settings::blinn);

		// Material Data
		Material& material = ResourceManager::currentMaterial;
		currentShader.setVec3("material.colour", material.colour);
		currentShader.setFloat("material.ambientAlbedo", material.ambientAlbedo);
		currentShader.setFloat("material.diffuseAlbedo", material.diffuseAlbedo);
		currentShader.setFloat("material.specularAlbedo", material.specularAlbedo);
		currentShader.setFloat("material.shininess", material.shininess);

		// Light Data
		LightSource& light = LightSources[0];
		currentShader.setVec3("light[0].posVec", light.posVec);
		currentShader.setVec3("light[0].dirVec", light.dirVec);
		currentShader.setVec3("light[0].colour", light.colour);
		currentShader.setBool("light[0].pointLight", light.pointLight);
		currentShader.setBool("light[0].activated", light.activated);

		LightSource& light2 = LightSources[1];
		currentShader.setVec3("light[1].posVec", light2.posVec);
		currentShader.setVec3("light[1].dirVec", light2.dirVec);
		currentShader.setVec3("light[1].colour", light2.colour);
		currentShader.setBool("light[1].pointLight", light2.pointLight);
		currentShader.setBool("light[1].activated", light2.activated);

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
		Shader phongShader("src/shaders/vertex_shader.vs", "src/shaders/phong_shader.fs");
		Shader defaultShader("src/shaders/vertex_shader.vs", "src/shaders/default_shader.fs");
		Shader lightShader("src/shaders/lightsource_shader.vs", "src/shaders/lightsource_shader.fs");
		loadShader("phongShader", phongShader);
		loadShader("defaultShader", defaultShader);
		loadShader("lightShader", lightShader);


		// load phong shader
		currentShader = getShader("phongShader");
		currentShader.use();
	};

	static void initialiseModels()
	{
		Model cubeModel("src/assets/cube/cube.obj");
		Model teapotModel("src/assets/teapot.obj");
		Model sphereModel("src/assets/sphere.obj");
		Model cowModel("src/assets/cow.obj");
		loadModel("cubeModel", cubeModel);
		loadModel("teapotModel", teapotModel);
		loadModel("sphereModel", sphereModel);
		loadModel("cowModel", cowModel);

		currentModel = getModel("cubeModel");
	}

	static void initialiseTextures()
	{	
		Texture cube_default_texture = Texture("src/assets/cube/default.png");
		loadTexture("default", cube_default_texture);

		Texture brick_texture = Texture("src/assets/brick.jpg");
		loadTexture("brick", brick_texture);

		currentTexture = getTexture("default");
		glBindTexture(GL_TEXTURE_2D, currentTexture.ID);
	}

	static void initialiseLightSources()
	{
		LightSource light1(getShader("lightShader"));
		light1.pointLight = true;
		light1.activated = true;
		light1.dirVec = glm::vec3(0.0f, 0.0f, 10.0f);
		light1.posVec = glm::vec3(0.0f);
		light1.colour = glm::vec3(1.0f);
		LightSources.push_back(light1);

		LightSource light2(getShader("lightShader"));
		light2.pointLight = true;
		light2.activated = true;
		light2.dirVec = glm::vec3(0.0f, 0.0f, 10.0f);
		light2.posVec = glm::vec3(10.0f);
		light2.colour = glm::vec3(1.0f);
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
		loadMaterial("custom", custom);
		currentMaterial = getMaterial("custom");

		// TODO Add More materials
		Material iron;
		iron.colour = glm::vec3(1.0f, 0.5f, 0.0f);
		iron.shininess = 128.0f;
		iron.ambientAlbedo = 0.5f;
		iron.diffuseAlbedo = 0.5f;
		iron.specularAlbedo = 0.5f;
		loadMaterial("iron", iron);


	}

};

#endif

