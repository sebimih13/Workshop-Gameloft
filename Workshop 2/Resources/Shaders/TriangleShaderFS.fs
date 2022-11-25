precision mediump float;

varying vec3 fs_color;
varying vec2 fs_uv;

uniform sampler2D u_texture;

void main()
{
	gl_FragColor = texture2D(u_texture, fs_uv);
}

