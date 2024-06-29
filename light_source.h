#ifndef LIGHT_SOURCE
#define LIGHT_SOURCE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"


class LightSource
{
public:
	Shader lightShader;

	glm::vec3 posVec;
	float radius;

	glm::vec3 lightColour;
	glm::vec3 ambientAlbedo;
	glm::vec3 diffuseAlbedo;
	glm::vec3 specularAlbedo;

	// Constructor
	LightSource()
	{
		initialise();
	}

	void drawLight(glm::mat4 projection, glm::mat4 view)
	{
		
		lightShader.use();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", glm::mat4(1.0f));
		lightShader.setVec3("lightPos", posVec);

		glPointSize(10);

		float point[] = { posVec.x, posVec.y, posVec.z };
		
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_DYNAMIC_DRAW);

		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point), (void*)0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
		return;
	}

	void loadLightSource()
	{
		return;
	}

private:
	unsigned int VAO, VBO;

	void initialise()
	{
		// Initialise Shader
		lightShader = Shader("src/shaders/lightsource_shader.vs", "src/shaders/lightsource_shader.fs");
		lightShader.use();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(0);
	};
};

#endif