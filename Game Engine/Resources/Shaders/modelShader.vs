attribute vec3 a_pos;
attribute vec2 a_uv;

varying vec3 fs_worldPos;
varying vec2 fs_uv;

uniform mat4 u_mvpMatrix;
uniform mat4 u_modelMatrix;

void main()
{
	vec4 worldPos = u_modelMatrix * vec4(a_pos, 1.0);
	fs_worldPos = vec3(worldPos.x, worldPos.y, worldPos.z);
	fs_uv = a_uv;

	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

