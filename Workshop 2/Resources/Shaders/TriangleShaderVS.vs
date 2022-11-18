attribute vec3 a_posL;
attribute vec3 a_color;

varying vec3 fs_color;

uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;
uniform mat4 u_modelMatrix;

void main()
{
	fs_color = a_color;
	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_posL, 1.0);
	// gl_Position = vec4(a_posL, 1.0) * u_modelMatrix * u_viewMatrix * u_projectionMatrix;
}

