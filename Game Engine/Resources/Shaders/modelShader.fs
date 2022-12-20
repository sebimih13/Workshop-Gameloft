precision mediump float;

varying vec3 fs_worldPos;
varying vec2 fs_uv;

uniform sampler2D u_texture;

// Fog
uniform float u_r;
uniform float u_R;
uniform vec3 u_fogColor;
uniform vec3 u_cameraPos;

void main()
{
	// texture color
	vec4 color_object = texture2D(u_texture, fs_uv);

	if (color_object.a <= 0.2)
	{
		discard;
	}

	// fog
	float fogAlpha = 0.0;
	float d = distance(fs_worldPos, u_cameraPos);
	if (d <= u_r)			// TODO : fara if => clamp
	{
		fogAlpha = 0.0;
	}
	else if (d <= u_R)
	{
		fogAlpha = (d - u_r) / (u_R - u_r);
	}
	else
	{
		fogAlpha = 1.0;
	}

	vec4 color = vec4(fogAlpha * u_fogColor, 1.0) + (1.0 - fogAlpha) * color_object;		// vec4(fogAlpha * u_fogColor, 1.0) - e 1.0 aici?

	gl_FragColor = color;
}

