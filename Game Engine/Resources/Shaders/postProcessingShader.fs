precision mediump float;

varying vec2 fs_uv;

uniform int u_grayscale;

uniform vec2 u_offsets[9];

uniform int u_blur;
uniform float u_blurKernel[9];

uniform int u_sharpen;
uniform float u_sharpenKernel[9];

uniform sampler2D u_texture_0;	// screen texture

void main()
{
	gl_FragColor = texture2D(u_texture_0, fs_uv);

	// sample from texture offsets
	vec3 samples[9];
	for (int i = 0; i < 9; i++)
	{
		samples[i] = vec3(texture2D(u_texture_0, fs_uv.st + u_offsets[i]));
	}

	// process effects
	if (u_grayscale == 1)
	{
		float average = 0.2126 * gl_FragColor.r + 0.7152 * gl_FragColor.g + 0.0722 * gl_FragColor.b;
		gl_FragColor = vec4(average, average, average, 1.0);
	}
	else if (u_blur == 1)
	{
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
		{
			col += samples[i] * u_blurKernel[i];
		}
		gl_FragColor = vec4(col, 1.0);
	}
	else if (u_sharpen == 1)
	{
		vec3 col = vec3(0.0);
		for (int i = 0; i < 9; i++)
		{
			col += samples[i] * u_sharpenKernel[i];
		}
		gl_FragColor = vec4(col, 1.0);
	}
}

