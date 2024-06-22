#version 430 core

out vec4 outputColour;

in vec3 FragPos;
in vec3 Normal;
in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform bool texture_setting;
uniform vec3 objectColour;

uniform vec3 lightPos;
uniform vec3 ambientColour;
uniform vec3 viewPos;



void main()
{
	vec3 NormalUnitVec = normalize(Normal);        // Normalised normal vectors
	vec3 lightDir = normalize(lightPos - FragPos); // Normalised light direction (optimisation)

	// Ambient
	vec3 lightColour = vec3(1.0f, 1.0f, 1.0f);
	float ambientAlbedo = 0.5f;

	// Diffuse
	float diff = max(dot(NormalUnitVec, lightDir), 0.0);
	float diffuseAlbedo = 0.5f;
	vec3 diffuse = diff * diffuseAlbedo * lightColour;

	// Specular
	float specularAlbedo = 0.5f;
	float shininess = 128.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularAlbedo * spec * lightColour;

	if (texture_setting)
	{
		vec3 colour = texture(texture1, textureCoordinates).rgb;
		outputColour = vec4((ambientColour * ambientAlbedo + diffuse + specular) * colour, 1.0f);
	}

	else
	{
		outputColour = vec4((ambientColour * ambientAlbedo + diffuse + specular) * objectColour, 1.0f);
	}
}