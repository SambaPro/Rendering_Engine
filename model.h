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

//#define MODEL_DEBUG 1;

class Model
{
public:
	std::vector<Mesh> meshes;

	/* Model Constructor*/
	Model() {};
	Model(const char* path)
	{
		loadModel(path);
	}

	/* Draws all meshes in model */
	void drawModel(Shader shader)
	{
		shader.use();

		for (int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].drawMesh();
		};
	}

private:

	void loadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return;
		}
		std::string directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	};

	void processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			try
			{
				#if MODEL_DEBUG
					std::cout << "node->mNumMeshes:" << node->mNumMeshes << std::endl;
					std::cout << "mNumVertices: " << mesh->mNumVertices << std::endl;
				#endif

				meshes.push_back(processMesh(mesh, scene));
			}
			catch(...)
			{
				std::cout << "Error: Mesh cannot be processed" << std::endl;
			}
		}
		// process child nodes if any
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	};

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;


		// Loop through mesh vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex{};
			// Process Vertex Vectors
			glm::vec3 vector {};
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.positionVec = vector;


			if (mesh->HasNormals())
			{
				// Process Normal Vectors
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normalVec = vector;
			}

			// Process Texture Coordinates
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec2 {};
				vec2.x = mesh->mTextureCoords[0][i].x;
				vec2.y = mesh->mTextureCoords[0][i].y;
				vertex.textureCoordinates = vec2;

			}

			else
				vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		

		// Process Indicies
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