attribute vec3 a_pos;

varying vec3 fs_coord;

uniform mat4 u_mvpMatrix;

void main()
{
	fs_coord = a_pos;
	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

