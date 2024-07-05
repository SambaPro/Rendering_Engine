#version 430 core

out vec4 fragColour;

struct Light
{
	vec3 pos;
	vec3 colour;
};

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

in vec3 FragPos;
in vec3 Normal;
in vec2 textureCoordinates;

uniform sampler2D texture1;
uniform vec3 viewPos;

uniform Light light;
uniform Material material;
uniform Settings settings;


vec3 getAmbient()
{
	return light.colour * material.ambientAlbedo;
}

vec3 getDiffuse(vec3 Normal_N, vec3 lightDir_N)
{
	float diff = max(dot(Normal_N, lightDir_N), 0.0);
	return diff * material.diffuseAlbedo * light.colour;
}

vec3 getSpecular(vec3 lightDir_N, vec3 viewDir, vec3 Normal_N, float shininess)
{
	float spec = 0.0;

	if (settings.blinn)
	{
		vec3 halfwayDir = normalize(lightDir_N + viewDir);
		spec = pow(max(dot(Normal_N, halfwayDir), 0.0), shininess);
	}

	else
	{
		vec3 reflectDir = reflect(-lightDir_N, Normal_N);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); // Specular Reflectanse
	}

	return material.specularAlbedo * spec * light.colour;
}



void main()
{
	// Normalise vectors
	vec3 Normal_N = normalize(Normal);
	vec3 lightDir_N = normalize(light.pos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);

	// Get Lighting Factors
	vec3 ambient = getAmbient();
	vec3 diffuse = getDiffuse(Normal_N, lightDir_N);
	vec3 specular = getSpecular(lightDir_N, viewDir, Normal_N, material.shininess);

	// Get surface colour/texture
	vec3 colour = material.colour;

	if (settings.texture_setting)
		colour = texture(texture1, textureCoordinates).rgb;


	// Get combined fragment colour
	fragColour = vec4((ambient + diffuse + specular) * colour, 1.0f);
}