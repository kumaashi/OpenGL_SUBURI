#version 450

attribute vec3 position;
attribute vec3 normal;
uniform   vec4 info;
uniform   mat4 proj;
uniform   mat4 view;
uniform   mat4 world;

varying   vec4 IncColor;
varying   vec4 IngColor;

void main() {
	mat4 wvp    = proj * view * world;
	vec3 p      = position;
	vec3 N      = normalize( (vec4(normal, 0.0)).xyz);
	vec4 rp     = wvp * vec4(p, 1.0);
	IncColor    = vec4(dot(normalize(vec3(0.5,1.0,1.0)), N));
	IngColor    = vec4(N, rp.z / rp.w);
	gl_Position = rp;
}


