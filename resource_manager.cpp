#include "resource_manager.h"

std::vector<Shader> ResourceManager::Shaders;
std::vector<Model> ResourceManager::Models;
std::vector<Texture> ResourceManager::Textures;

Shader ResourceManager::currentShader {};
Model ResourceManager::currentModel;
Texture ResourceManager::currentTexture;