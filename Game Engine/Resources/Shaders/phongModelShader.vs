attribute vec3 a_pos;
attribute vec3 a_norm;
attribute vec2 a_uv;

varying vec3 fs_worldPos;
varying vec3 fs_norm;
varying vec2 fs_uv;

uniform mat4 u_mvpMatrix;
uniform mat4 u_modelMatrix;

void main()
{
	fs_worldPos = vec3(u_modelMatrix * vec4(a_pos, 1.0));
	fs_norm = a_norm;
	fs_uv = a_uv;

	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

