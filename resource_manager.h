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
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;
	static std::map<std::string, Model> Models;

	static Shader currentShader;
	static Model currentModel;
	static Texture currentTexture;

	static void loadShader(std::string name, Shader shader)
	{
		Shaders[name] = shader;
		std::cout << "Shader ID: " << Shaders[name].ID << " loaded successfully" << std::endl;
	}

	static Shader getShader(std::string name)
	{
		return Shaders[name];
	}


	static void loadModel(std::string name, Model model)
	{
		Models[name] = model;
		std::cout << "Model loaded successfully" << std::endl;
	}

	static Model getModel(std::string name)
	{
		return Models[name];
	}


	static void loadTexture(std::string name, Texture texture)
	{
		Textures[name] = texture;
		std::cout << "Texture ID: " << Textures[name].ID << " loaded successfully" << std::endl;
	}

	static Texture getTexture(std::string name)
	{
		return Textures[name];
	}

	static void initialise()
	{
		//TODO put loading into here
		return;
	}
};

#endif

