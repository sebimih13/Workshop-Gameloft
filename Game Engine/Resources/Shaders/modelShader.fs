precision mediump float;

varying vec2 fs_uv;

uniform sampler2D u_texture;

void main()
{
	vec4 color = texture2D(u_texture, fs_uv);

	if (color.a <= 0.2)
	{
		discard;
	}

	gl_FragColor = color;
}

