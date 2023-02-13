precision mediump float;

varying vec3 fs_worldPos;
varying vec3 fs_norm;

// Object color
uniform vec3 u_color;

// Skybox texture
uniform samplerCube u_texture_0;

// Camera position
uniform vec3 u_viewPos;

void main()
{
	vec3 vectCam = normalize(fs_worldPos - u_viewPos);
	vec3 dirReflect = reflect(vectCam, normalize(fs_norm));
	vec4 c_reflexie = textureCube(u_texture_0, dirReflect);	

	// Final result
	gl_FragColor = 0.5 * c_reflexie + 0.5 * vec4(u_color, 1.0);
	gl_FragColor.a = 1.0;
}

