attribute vec3 a_pos;
attribute vec3 a_norm;

varying vec3 fs_worldPos;
varying vec3 fs_norm;

uniform mat4 u_mvpMatrix;
uniform mat4 u_modelMatrix;

void main()
{
	fs_worldPos = vec3(u_modelMatrix * vec4(a_pos, 1.0));
	fs_norm = vec3(u_modelMatrix * vec4(a_norm, 1.0));

	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

