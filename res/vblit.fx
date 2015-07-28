uniform   sampler2D   tex;
attribute vec3 pos;
varying   vec4 cColor;
varying   vec4 gColor;

void main() {
	vec3 p      = pos;
	gl_Position = vec4(p, 1.0);
}


