attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec2 a_uv;

varying vec3 fs_color;
varying vec2 fs_uv;

uniform mat4 u_mvpMatrix;

void main()
{
	fs_uv = a_uv;
	fs_color = a_color;
	gl_Position = u_mvpMatrix * vec4(a_posL, 1.0);
}

