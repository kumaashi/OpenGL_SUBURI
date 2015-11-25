#version 450

uniform  mat4 view;
uniform  mat4 proj;
uniform  mat4 world;
in       vec3 pos;
in       vec3 nor;
/*
out      vec4 out_color;
out      vec3 out_pos;
out      vec3 out_nor;
void main() {
	mat4 wvp    = proj * view * world;
	vec3 N      = normalize( (vec4(nor, 0.0)).xyz);
	out_color   = vec4(1.0);
	out_pos     = (world * vec4(pos, 1.0)).xyz;
	out_nor     = nor;
	gl_Position = wvp * vec4(pos, 1.0);
}
*/
varying      vec4 out_color;
varying      vec3 out_pos;
varying      vec3 out_nor;
void main() {
	mat4 wvp    = proj * view * world;
	vec3 N      = normalize( (vec4(nor, 0.0)).xyz);
	out_color   = vec4(1.0);
	out_pos     = (world * vec4(pos, 1.0)).xyz;
	out_nor     = nor;
	gl_Position = wvp * vec4(pos, 1.0);
}

