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
	//std::map<std::string, Texture> Textures;
	//std::map<std::string, Model> Models;

	static Shader currentShader;

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

private:
	ResourceManager() { }
};

#endif

