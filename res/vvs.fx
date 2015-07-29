#version 150

in        vec3 pos;
in        vec3 nor;
uniform   vec4 info;
uniform   mat4 proj;
uniform   mat4 view;
uniform   mat4 world;
out       vec4 out_color;

void main() {
	mat4 wvp    = proj * view * world;
	vec3 p      = pos;
	vec3 N      = normalize( (vec4(nor, 0.0)).xyz);
	vec4 rp     = wvp * vec4(p, 1.0);
	out_color   = vec4(rp.z / rp.w);
	gl_Position = rp;
}


