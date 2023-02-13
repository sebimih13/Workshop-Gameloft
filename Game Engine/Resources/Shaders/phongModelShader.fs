precision mediump float;

// const values for Lights
#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_POINT_LIGHTS 1
#define MAX_SPOTLIGHTS 1

varying vec3 fs_worldPos;
varying vec3 fs_norm;
varying vec2 fs_uv;

// Lights structs
struct DirectionalLight
{
	vec3 direction;	

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Spotlight
{
	vec3 position;
	vec3 direction;

    float constant;
    float linear;
    float quadratic;
	
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// Lights
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Spotlight spotlights[MAX_SPOTLIGHTS];

// Object color
uniform vec3 u_color;

// Camera position
uniform vec3 u_viewPos;

// Calculate Light Functions
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(fs_norm);
	vec3 viewDir = normalize(u_viewPos - fs_worldPos);

	// Result
	vec3 result = vec3(0.0, 0.0, 0.0);

	// Directional Lights
	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	{
		result += calculateDirectionalLight(directionalLights[i], norm, viewDir);
	}

	// Point Lights
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		result += calculatePointLight(pointLights[i], norm, fs_worldPos, viewDir);
	}

	// Spotlights
	for (int i = 0; i < MAX_SPOTLIGHTS; i++)
	{
		result += calculateSpotlight(spotlights[i], norm, fs_worldPos, viewDir);
	}
	
	// Final result
	gl_FragColor = vec4(result, 1.0);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// ambient
	vec3 ambient = light.ambient * u_color;

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * u_color;
	
	// specular
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = light.specular * spec * u_color;

	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	// ambient
	vec3 ambient = light.ambient * u_color;

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * u_color;

	// specular
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = light.specular * spec * u_color;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// ambient 
	vec3 ambient = light.ambient * u_color;

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * u_color;

	// specular
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = light.specular * spec * u_color;

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}

// TODO : uniform SpecPower		- proprietatea obictului (nu a luminii)

