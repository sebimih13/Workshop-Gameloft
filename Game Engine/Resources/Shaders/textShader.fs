precision mediump float;

varying vec2 fs_uv;

uniform vec3 u_color;
uniform sampler2D u_texture_0;

void main()
{
	float alpha = texture2D(u_texture_0, fs_uv);

	gl_FragColor = vec4(1.0, 1.0, 1.0, alpha) * color;
}

