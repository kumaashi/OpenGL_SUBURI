#version 450

varying   vec4 out_color;
varying   vec3 out_pos;
varying   vec3 out_nor;

void main() {
	gl_FragData[0] = vec4(1,2,3,4) * 0.1;
	gl_FragData[1] = vec4(out_pos, 1.0);
	gl_FragData[2] = vec4(out_nor, 1.0);
	gl_FragData[3] = vec4(out_color.xyz, 1.0);
}
