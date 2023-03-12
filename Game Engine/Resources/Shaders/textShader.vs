attribute vec3 a_pos;
attribute vec2 a_uv;

varying vec2 fs_uv;

void main()
{
	fs_uv = a_uv;

	gl_Position = vec4(a_pos.xy, 0.0, 1.0);
}

