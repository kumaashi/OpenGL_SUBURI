#version 450

uniform   vec4 info;
uniform   mat4 proj;
uniform   mat4 view;
uniform   mat4 world;
in        vec3 pos;
in        vec3 nor;
out       vec4 cColor;
out       vec4 gColor;

void main() {
	mat4 wvp  = proj * view * world;
	vec3 p    = pos;
	//vec3 N    = normalize((view * vec4(nor, 0.0)).xyz);
	vec3 N    = normalize((world * vec4(nor, 0.0)).xyz);
	vec4 rp   = wvp * vec4(p, 1.0);
	cColor    = vec4(dot(normalize(vec3(0.5,1.0,1.0)), N));
	gColor    = vec4(N, rp.z / rp.w);
	gl_Position = rp;
}


