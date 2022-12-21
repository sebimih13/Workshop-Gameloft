precision mediump float;

varying vec2 fs_uv;

uniform float u_time;			// TODO : de transmis
uniform float u_dispMax;		// TODO : de transmis

uniform sampler2D u_texture_0;	// DisplacementMap.tga
uniform sampler2D u_texture_1;	// fire3.tga
uniform sampler2D u_texture_2;	// fire_mask.tga

void main()
{
	// displacement
	vec2 disp = texture2D(u_texture_0, vec2(fs_uv.x, fs_uv.y + u_time)).rg;
	vec2 offset = vec2((disp.x * 2.0 - 1.0) * u_dispMax, (disp.y * 2.0 - 1.0) * u_dispMax);
	vec2 uvDisplaced = fs_uv + offset;

	// texture color
	vec4 c_fire = texture2D(u_texture_1, uvDisplaced);

	// alpha
	vec4 c_alpha = texture2D(u_texture_2, fs_uv);
	c_fire.a = c_fire.a * c_alpha.r;

	gl_FragColor = c_fire;
}

