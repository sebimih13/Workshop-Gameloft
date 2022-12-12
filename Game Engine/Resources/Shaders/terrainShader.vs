attribute vec3 a_pos;
attribute vec2 a_uv;

varying vec2 fs_uv_1;
varying vec2 fs_uv_2;

uniform mat4 u_mvpMatrix;
uniform vec3 u_height;
uniform float u_nrCelule;

// TODO : asa pun in .vs + .fs
uniform sampler2D u_texture_3;	// terrain_blend_map.tga

void main()
{
	fs_uv_1 = a_uv;
	fs_uv_2 = a_uv / u_nrCelule;

	vec4 c_blend = texture2D(u_texture_3, a_uv / u_nrCelule);

	vec4 pos_nou = vec4(a_pos, 1.0);
	pos_nou.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;

	gl_Position = u_mvpMatrix * pos_nou;
}

