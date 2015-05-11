#version 130

uniform vec4 info;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 world;
varying vec4 cColor;
varying vec4 gColor;

void main() {
	//gl_FragColor = vec4(gColor);
	gl_FragData[0] = vec4(gColor);
}


