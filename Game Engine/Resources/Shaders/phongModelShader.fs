precision mediump float;

varying vec3 fs_worldPos;
varying vec3 fs_norm;
varying vec2 fs_uv;

// object color
uniform vec3 u_color;

// camera position
uniform vec3 u_viewPos;

// lights Count
uniform int u_lightsCount;		// TODO : nu pot sa folosesc uniform in for ???

// light position
uniform vec3 u_lightPos[10];

// ambient light
uniform vec3 u_ambientColor;
uniform float u_ambientStrength;

// diffuse
uniform vec3 u_diffuseColor[10];
uniform float u_diffuseStrength[10];

// specular
uniform vec3 u_specularColor[10];
uniform float u_specularStrength[10];

void main()
{
	// ambient
	vec3 ambient = u_ambientStrength * u_ambientColor;

	// Diffuse + Specular in for
	vec3 mixedLights = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < 10; i++)				// TODO : inlocuieste cu u_lightsCount
	{
		// diffuse
		vec3 norm = normalize(fs_norm);
		vec3 lightDir = normalize(u_lightPos[i] - fs_worldPos);

		float diff = max(dot(fs_norm, lightDir), 0.0);
		vec3 diffuse = u_diffuseStrength[i] * diff * u_diffuseColor[i];

		// specular	
		vec3 viewDir = normalize(u_viewPos - fs_worldPos);
		vec3 reflectDir = reflect(-lightDir, norm);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
		vec3 specular = u_specularStrength[i] * spec * u_specularColor[i];

		mixedLights += diffuse + specular;
	}

	// result
	vec3 result = (ambient + (1.0 - u_ambientStrength) * mixedLights) * u_color;
	gl_FragColor = vec4(result, 1.0);
}

// TODO : put texture 

