#version 430 core

out vec4 FragColour;

in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform bool texture_setting;
uniform vec3 objectColour;


void main()
{
	
	if (texture_setting==true)
	{
		FragColour = texture(texture1, textureCoordinates);
	}
	
	else
	{
		FragColour = vec4(objectColour, 1.0f);
	}
	

}