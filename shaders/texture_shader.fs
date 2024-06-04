#version 430 core

in vec2 textureCoordinates;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

//uniform sampler2D default;


void main()
{
	FragColor = mix(texture(texture1, textureCoordinates), texture(texture2, textureCoordinates), 0.2);
}