#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
public:
	glm::vec3 colour;
	float ambientAlbedo;
	float diffuseAlbedo;
	float specularAlbedo;
	float shininess;
};


#endif
