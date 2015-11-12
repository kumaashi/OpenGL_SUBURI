#version 450

in vec4 out_color;
in vec3 out_pos;

void main() {
	//gl_FragColor = vec4(out_color.xyz, 1.0);
	//gl_FragColor = vec4(gl_FragCoord.z / gl_FragCoord.w);
	//gl_FragColor = vec4(out_color.xyz, 1.0);

	//gl_FragColor = vec4(out_color.xyz, gl_FragCoord.z / gl_FragCoord.w);
	gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
	gl_FragData[1] = vec4(0.0, 1.0, 0.0, 1.0);
	gl_FragData[2] = vec4(0.0, 0.0, 1.0, 1.0);
	gl_FragData[3] = vec4(0.0, 0.0, 0.0, 1.0);
}

