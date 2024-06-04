#include "resource_manager.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;
std::map<std::string, Texture> ResourceManager::Textures;

Shader ResourceManager::currentShader;
Model ResourceManager::currentModel;
Texture ResourceManager::currentTexture;