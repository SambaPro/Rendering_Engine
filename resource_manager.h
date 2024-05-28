#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>

#include "texture.h"
#include "shader.h"
#include "model.h"

class ResourceManager
{
public:
	static std::vector<Shader> Shaders;
	static std::vector<Texture> Textures;
	static std::vector<Model> Models;

	static Shader currentShader;
	static Model currentModel;
	static Texture currentTexture;

	static void loadShader(Shader shader)
	{
		Shaders.push_back(shader);
		std::cout << Shaders.back().ID;
		std::cout << "Shader loaded Successfully" << std::endl;
	}

	static Shader getShader(int index)
	{
		return Shaders[index];
	}


	static void loadModel(Model model)
	{
		Models.push_back(model);
		std::cout << "Model loaded sucessfully" << std::endl;
	}

	static Model getModel(int index)
	{
		return Models[index];
	}


	static void loadTexture(Texture texture)
	{
		Textures.push_back(texture);
		std::cout << "Texture loaded sucessfully" << std::endl;
		std::cout << Textures[0].ID << std::endl;
	}

	static Texture getTexture(int index)
	{
		return Textures[index];
	}

	static void changeTexture(int index)
	{

	}


private:
	ResourceManager() { }
};

#endif

