#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <iostream>



struct Vertex
{
	glm::vec3 positionVec;
	glm::vec3 normalVec;
	glm::vec2 textureCoordinates;
};


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Constructor
	Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices)
	{
		vertices = Vertices;
		indices = Indices;

		if (Vertices.size() > 0)
		{
			initialiseMesh();
		}

		else
		{
			std::cout << "Failed to load mesh, no vertices to load" << std::endl;
		}
		
	};


	void drawMesh()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	};

private:
	unsigned int VAO, VBO, EBO; // generate ID for each buffer

	void initialiseMesh()
	{
		std::cout << "Initialising Mesh with vertices:" << vertices.size() << std::endl;
		// Create Buffer Objects
		glGenVertexArrays(1, &VAO); // Vertex Array Object
		glGenBuffers(1, &VBO);      // Vertex Buffer Object
		glGenBuffers(1, &EBO);      // Element Buffer Object

		// Bind Buffer Objects to Context
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		// Specify storage space for each buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); // target, size, data, usage
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Load Vertex Coordinates
		glEnableVertexAttribArray(0); // Enable Automatic Filling
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // index, size, type, normalised, stride, pointer

		// Load Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalVec));

		// Load Texture Coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

		glBindVertexArray(0); // Unbind

	};
};


#endif