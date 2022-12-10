attribute vec3 a_pos;
attribute vec2 a_uv;

varying vec2 fs_uv;

uniform mat4 u_mvpMatrix;

void main()
{
	fs_uv = a_uv;
	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

// TODO : de refacuat

