#version 430 core

out vec4 fragColour;

struct Light
{
	bool pointLight;
	bool activated;
	vec3 posVec;
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

uniform Light light[2];
uniform Material material;
uniform Settings settings;

int MAX_LIGHTS = 2;


vec3 getAmbient(Light lightn)
{
	return lightn.colour * material.ambientAlbedo;
}

vec3 getDiffuse(Light lightn, vec3 Normal_N, vec3 lightDir_N)
{
	float diff = max(dot(Normal_N, lightDir_N), 0.0);
	return diff * material.diffuseAlbedo * lightn.colour;
}

vec3 getSpecular(Light lightn, vec3 lightDir_N, vec3 viewDir, vec3 Normal_N, float shininess)
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

	return material.specularAlbedo * spec * lightn.colour;
}


void main()
{
	vec3 result = vec3(0.0);

	// Normalise vectors
	vec3 Normal_N = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// for each lightsource loaded
	for (int i=0; i < MAX_LIGHTS; ++i)
	{
		if (!light[i].activated)
			continue;

		vec3 lightDir_N = normalize(light[i].posVec - FragPos);

		if (!light[i].pointLight)
			lightDir_N = normalize(light[i].posVec);

		// Get Lighting Factors
		vec3 ambient = getAmbient(light[i]);
		vec3 diffuse = getDiffuse(light[i], Normal_N, lightDir_N);
		vec3 specular = getSpecular(light[i], lightDir_N, viewDir, Normal_N, material.shininess);

		result += (ambient + diffuse + specular);
	}

	// Get surface colour/texture
	vec3 colour = material.colour;

	if (settings.texture_setting)
		colour = texture(texture1, textureCoordinates).rgb;

	// Get combined fragment colour
	fragColour = vec4(result * colour, 1.0f);
}