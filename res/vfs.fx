#version 450

in vec4 out_color;
in vec3 out_pos;

void main() {
	gl_FragColor = vec4(out_color.xyz, 1.0);
	gl_FragColor = vec4(1.0);
}

