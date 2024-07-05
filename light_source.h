#ifndef LIGHT_SOURCE_H
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
	bool pointLight;

	// Direction light properties
	glm::vec3 dirVec;

	// Point light properties
	glm::vec3 posVec;
	glm::vec3 colour;

	// Constructor
	LightSource(Shader shader)
	{
		initialise(shader);
	}

	void drawLight(Shader shader, glm::mat4 projection, glm::mat4 view)
	{
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", glm::mat4(1.0f));
		shader.setVec3("lightPos", posVec);

		glPointSize(10);

		float point[] = { posVec.x, posVec.y, posVec.z };
		
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_DYNAMIC_DRAW);

		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point), (void*)0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

private:
	unsigned int VAO, VBO;

	void initialise(Shader shader)
	{
		pointLight = true;

		shader.use();
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(0);
	};
};

#endif