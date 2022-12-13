precision mediump float;

varying vec3 fs_coord;

uniform samplerCube u_texture_0;

void main()
{
	gl_FragColor = textureCube(u_texture_0, fs_coord);
}

