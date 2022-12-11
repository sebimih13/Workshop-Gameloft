precision mediump float;

varying vec2 fs_uv_1;
varying vec2 fs_uv_2;

uniform sampler2D u_texture_0;	// dirt.tga
uniform sampler2D u_texture_1;	// rock.tga
uniform sampler2D u_texture_2;	// grass.tga
uniform sampler2D u_texture_3;	// terrain_blend_map.tga

void main()
{
	vec4 c_dirt  = texture2D(u_texture_0, fs_uv_1);
	vec4 c_rock  = texture2D(u_texture_1, fs_uv_1);
	vec4 c_grass = texture2D(u_texture_2, fs_uv_1);
	vec4 c_blend = texture2D(u_texture_3, fs_uv_2);

	vec4 c_final = c_blend.r * c_rock + c_blend.g * c_grass + c_blend.b * c_dirt;
	c_final.a = 1.0;

	gl_FragColor = c_final;
}

