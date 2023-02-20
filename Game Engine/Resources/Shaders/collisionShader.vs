attribute vec3 a_pos;

uniform mat4 u_mvpMatrix;

void main()
{
	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

