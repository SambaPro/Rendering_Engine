#version 430 core

out vec4 fragColour;

in vec2 textureCoordinates;

struct Material
{
	vec3 colour;
	float ambientAlbedo;
	float diffuseAlbedo;
	float specularAlbedo;
	float shininess;
};

struct Settings
{
	bool texture_setting;
	bool blinn;
};

uniform Material material;
uniform sampler2D texture1;
uniform Settings settings;


void main()
{
	
	if (settings.texture_setting==true)
	{
		fragColour = texture(texture1, textureCoordinates);
	}
	
	else
	{
		fragColour = vec4(material.colour, 1.0f);
	}

}