precision mediump float;

varying vec3 fs_worldPos;
varying vec3 fs_norm;
varying vec2 fs_uv;

// object color
uniform vec3 u_color;

// camera position
uniform vec3 u_viewPos;

// ambient light
uniform vec3 u_ambientColor;
uniform float u_ambientStrength;

// diffuse
uniform vec3 u_diffuseColor;
uniform float u_diffuseStrength;

// specular
uniform vec3 u_specularColor;
uniform float u_specularStrength;

// light position
uniform vec3 u_lightPos;

void main()
{
	// ambient
	vec3 ambient = u_ambientStrength * u_ambientColor;

	// diffuse
	vec3 norm = normalize(fs_norm);
	vec3 lightDir = normalize(u_lightPos - fs_worldPos);

	float diff = max(dot(fs_norm, lightDir), 0.0);
	vec3 diffuse = u_diffuseStrength * diff * u_diffuseColor;

	// specular	
	vec3 viewDir = normalize(u_viewPos - fs_worldPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = u_specularStrength * spec * u_specularColor;

	// result
	vec3 result = (ambient + diffuse + specular) * u_color;
	gl_FragColor = vec4(result, 1.0);
}

// TODO : put texture 

