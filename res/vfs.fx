#version 150

uniform vec4 info;
in           vec4 color;

void main() {
	gl_FragColor = vec4(color);
}


