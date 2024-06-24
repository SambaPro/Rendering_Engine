#include "resource_manager.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;
std::map<std::string, Texture> ResourceManager::Textures;

Shader ResourceManager::currentShader;
Model ResourceManager::currentModel;
Texture ResourceManager::currentTexture;


void ResourceManager::loadShader(std::string name, Shader shader)
{
	ResourceManager::Shaders[name] = shader;
	std::cout << "Shader ID: " << ResourceManager::Shaders[name].ID << " loaded successfully" << std::endl;
}

Shader ResourceManager::getShader(std::string name)
{
	return ResourceManager::Shaders[name];
}

void ResourceManager::loadModel(std::string name, Model model)
{
	ResourceManager::Models[name] = model;
	std::cout << "Model loaded successfully" << std::endl;
}

Model ResourceManager::getModel(std::string name)
{
	return ResourceManager::Models[name];
}


void ResourceManager::loadTexture(std::string name, Texture texture)
{
	ResourceManager::Textures[name] = texture;
	std::cout << "Texture ID: " << ResourceManager::Textures[name].ID << " loaded successfully" << std::endl;
}

Texture ResourceManager::getTexture(std::string name)
{
	return ResourceManager::Textures[name];
}

void ResourceManager::initialise()
{
	//TODO put loading into here
	return;
}