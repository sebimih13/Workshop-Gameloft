attribute vec3 a_posL;
attribute vec3 a_color;

varying vec3 fs_color;

uniform mat4 u_mvpMatrix;

void main()
{
	fs_color = a_color;
	gl_Position = u_mvpMatrix * vec4(a_posL, 1.0);
}

