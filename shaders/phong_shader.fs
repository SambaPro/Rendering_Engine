#version 430 core

out vec4 fragColour;

struct Light
{
	vec3 pos;
	vec3 colour;
	float ambientAlbedo;
	float diffuseAlbedo;
	float specularAlbedo;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform bool texture_setting;
uniform vec3 objectColour;
uniform vec3 viewPos;


uniform Light light;

void main()
{
	vec3 NormalUnitVec = normalize(Normal);        // Normalised normal vectors
	vec3 lightDir = normalize(light.pos - FragPos); // Normalised light direction (optimisation)


	// Diffuse
	float diff = max(dot(NormalUnitVec, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuseAlbedo * light.colour;

	// Specular
	//float specularAlbedo = 0.5f;
	float shininess = 64.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = light.specularAlbedo * spec * light.colour;

	if (texture_setting)
	{
		vec3 colour = texture(texture1, textureCoordinates).rgb;
		fragColour = vec4((light.colour * light.ambientAlbedo + diffuse + specular) * colour, 1.0f);
	}

	else
	{
		fragColour = vec4((light.colour * light.ambientAlbedo + diffuse + specular) * objectColour, 1.0f);
	}
}