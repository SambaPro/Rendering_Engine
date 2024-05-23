#include "resource_manager.h"

std::vector<Shader> ResourceManager::Shaders;
std::vector<Model> ResourceManager::Models;

Shader ResourceManager::currentShader {};
Model ResourceManager::currentModel;