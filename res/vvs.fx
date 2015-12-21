#version 410

attribute  vec3 pos;
attribute  vec3 nor;

uniform    mat4 view;
uniform    mat4 proj;
uniform    mat4 world;

varying    vec4 out_color;
varying    vec3 out_nor;
varying    vec4 out_pos;

void main() {
	out_color   = vec4(1.0);
	out_pos     = proj * view * world * vec4(pos, 1.0);
	out_nor     = nor;
	gl_Position = out_pos;
}

