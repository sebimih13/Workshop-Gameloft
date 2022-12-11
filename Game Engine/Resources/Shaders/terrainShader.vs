attribute vec3 a_pos;
attribute vec2 a_uv;

varying vec2 fs_uv_1;
varying vec2 fs_uv_2;

uniform mat4 u_mvpMatrix;
uniform float u_nrCelule;

void main()
{
	fs_uv_1 = a_uv;
	fs_uv_2 = a_uv / u_nrCelule;	// TODO : corect?
	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

