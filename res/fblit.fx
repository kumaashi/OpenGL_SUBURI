#version 450

uniform vec4        info;
uniform vec4        info2;
uniform sampler2D   tex0;
uniform sampler2D   tex1;

void main() {
	vec2  uv     = (gl_FragCoord.xy / info2.xy);
	gl_FragData[0] = vec4(1,2,3,4) * 0.1;
	gl_FragData[1] = vec4(1,2,3,4) * 0.1;

	gl_FragData[0] = texture2D(tex1, uv);
	gl_FragData[1] = texture2D(tex1, uv);
}


