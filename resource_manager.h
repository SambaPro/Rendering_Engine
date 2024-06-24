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

	static void loadShader(std::string name, Shader shader);
	static Shader getShader(std::string name);
	static void loadModel(std::string name, Model model);
	static Model getModel(std::string name);
	static void loadTexture(std::string name, Texture texture);
	static Texture getTexture(std::string name);
	static void initialise();

};

#endif

