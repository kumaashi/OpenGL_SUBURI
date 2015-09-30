#version 450

uniform vec4        info;
uniform vec4        info2;
uniform sampler2D   tex;

void main() {
	vec2  uv     = (gl_FragCoord.xy / info.xy);
	gl_FragColor = texture2D(tex, uv);
}


