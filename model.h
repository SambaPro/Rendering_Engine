#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "shader.h"
#include "mesh.h"

class Model
{
public:
	//std::vector<Texture> loaded_textures;
	std::vector<Mesh> meshes;
	std::string directory;

	/* Model Constructor*/
	Model() {};
	Model(const char* path)
	{
		loadModel(path);
	}

	/* Draws all meshes in model */
	void drawModel(Shader& shader)
	{
		for (int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].drawMesh(shader);
		};
	}

private:
	std::string dictionary;

	void loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	};

	void processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	};

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		/* Loop through mesh vertices*/
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex{};
			/* Process Vertex Vectors */
			glm::vec3 vector {};
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.positionVec = vector;

			if (mesh->HasNormals())
			{
				/* Process Normal Vectors*/
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normalVec = vector;
			}

			else // Generate Normals from faces TODO
			{
				//std::cout << "No normals found" << std::endl;
				vector.x = 0;
				vector.y = 0;
				vector.z = 0;
				vertex.normalVec = vector;
			}

			/* Process Texture Coordinates */
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec2 {};
				vec2.x = mesh->mTextureCoords[0][i].x;
				vec2.y = mesh->mTextureCoords[0][i].y;
				vertex.textureCoordinates = vec2;

				vertices.push_back(vertex);
			}

			else
				vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
		}

		/* Process Indicies */
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		return Mesh(vertices, indices);
	};
};

#endif