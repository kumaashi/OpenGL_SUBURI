#version 450

out      vec4 fs_color;
out      vec3 fs_pos;
out      vec3 fs_nor;

void main() {
	gl_FragData[0] = vec4(fs_color.xyz, 1.0);
	gl_FragData[1] = vec4(fs_pos, 1.0);
	gl_FragData[2] = vec4(fs_nor, 1.0);
	gl_FragData[3] = vec4(fs_color.xyz, 1.0);
}

