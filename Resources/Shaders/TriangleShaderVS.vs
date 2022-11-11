attribute vec3 a_posL;
attribute vec3 a_color;

varying vec3 fs_color;

uniform mat4 u_rotMatrix;

void main()
{
	fs_color = a_color;
	vec4 posL = vec4(a_posL, 1.0) * u_rotMatrix;
	gl_Position = posL;
}

