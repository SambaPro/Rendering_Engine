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
private:
	unsigned int VAO, VBO;

public:
	bool activated;
	bool orbit;
	float radius;
	bool pointLight;
	float orbit_speed;

	glm::vec3 posVec;
	glm::vec3 colour;

	// Constructor
	LightSource(Shader shader)
	{
		initialise(shader);
	}

	void drawLight(Shader shader, glm::mat4 projection, glm::mat4 view)
	{
		if (!activated)
			return;
		
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", glm::mat4(1.0f));
		shader.setVec3("colour", colour);


		glPointSize(10);

		float point[] = { posVec.x, posVec.y, posVec.z };


		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_DYNAMIC_DRAW);

		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(point), (void*)0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	};

	void processLight()
	{
		if (!(orbit && activated))
			return;

		float time = static_cast<float>(glfwGetTime());

		posVec.x = radius * glm::sin(orbit_speed * time);
		posVec.y = radius * glm::sin(orbit_speed * time);
		posVec.z = radius * glm::cos(orbit_speed * time);

	}

private:
	void initialise(Shader shader)
	{
		orbit = false;
		pointLight = true;
		orbit_speed = 1.0f;
		radius = 10.0f;

		colour = glm::vec3(1.0f);

		shader.use();
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(0);
	}
};

#endif