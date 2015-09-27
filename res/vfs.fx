
uniform vec4 info;
varying vec4 fs_color;
varying vec3 fs_pos;

void main() {
	gl_FragColor = vec4(fs_color.xyz, 1.0);
	gl_FragColor = vec4(1.0);
}


