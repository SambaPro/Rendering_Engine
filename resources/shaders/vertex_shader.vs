#version 430 core

layout (location = 0) in vec3 aPos;                  // Vertex Positions from buffer using index 0
layout (location = 1) in vec3 aNormal;               // Normal Coordinates
layout (location = 2) in vec2 atextureCoordinates;   // Texture Coordinates

out vec3 FragPos;
out vec3 Normal;
out vec2 textureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    Normal = aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0f)); // convert from homogenous to cartesian 
    textureCoordinates = atextureCoordinates;
}