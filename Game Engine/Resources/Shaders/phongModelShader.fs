precision mediump float;

varying vec3 fs_worldPos;
varying vec3 fs_norm;
varying vec2 fs_uv;

// object color
uniform vec3 u_color;

// ambient light
uniform vec3 u_lightColor;			// TODO : rename : u_lightColor = culoarea pt ambient light
uniform float u_ambientStrength;

// light position
uniform vec3 u_lightPos;
// TODO : USE LIGHT COLOR	-> deocamdata pt test folosim aceasi culoare ca ambient light

uniform vec3 u_viewPos;

void main()
{
	// ambient
	vec3 ambient = u_ambientStrength * u_lightColor;

	// diffuse
	vec3 norm = normalize(fs_norm);
	vec3 lightDir = normalize(u_lightPos - fs_worldPos);

	float diff = max(dot(fs_norm, lightDir), 0.0);
	vec3 diffuse = diff * u_lightColor;					// TODO : u_lightColor -> schimba in diffuseColor

	// specular
	float specularStrength = 0.5;		// TODO : uniform
	
	vec3 viewDir = normalize(u_viewPos - fs_worldPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * u_lightColor;			// TODO : u_lightColor -> schimba in specularColor


	// result
	vec3 result = (ambient + diffuse + specular) * u_color;
	gl_FragColor = vec4(result, 1.0);
}

// TODO : put texture 

