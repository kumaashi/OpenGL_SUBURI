uniform   sampler2D   tex;
in        vec3 pos;
in        vec3 nor;
varying   vec4 cColor;
varying   vec4 gColor;

void main() {
	vec3 p      = pos;
	gl_Position = vec4(p, 1.0);
}


