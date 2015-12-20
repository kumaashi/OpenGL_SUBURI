#version 410

varying vec4  fs_color;
varying vec3  fs_nor;
varying vec4  fs_pos;

void main() {
	gl_FragData[0] = vec4(1,2,3,4) * 0.1;
	gl_FragData[1] = fs_pos;
	gl_FragData[2] = vec4(fs_nor, 1.0);
	gl_FragData[3] = fs_color;
}
