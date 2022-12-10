attribute vec3 a_pos;

varying vec3 fs_color;

uniform vec3 u_color;
uniform mat4 u_mvpMatrix;

void main()
{
	fs_color = u_color;
	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

