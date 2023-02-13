attribute vec3 a_pos;
attribute vec3 a_norm;
attribute vec3 a_binorm;
attribute vec3 a_tgt;
attribute vec2 a_uv;

varying vec3 fs_worldPos;
varying vec2 fs_uv;
varying mat3 fs_TBN;

uniform mat4 u_mvpMatrix;
uniform mat4 u_modelMatrix;

void main()
{
	fs_worldPos = vec3(u_modelMatrix * vec4(a_pos, 1.0));
	fs_uv = a_uv;

	// create TBN matrix
	vec3 T = normalize(vec3(u_modelMatrix * vec4(a_tgt, 0.0)));
	vec3 B = normalize(vec3(u_modelMatrix * vec4(a_binorm, 0.0)));
	vec3 N = normalize(vec3(u_modelMatrix * vec4(a_norm, 0.0)));

	fs_TBN = mat3(T, B, N);

	gl_Position = u_mvpMatrix * vec4(a_pos, 1.0);
}

