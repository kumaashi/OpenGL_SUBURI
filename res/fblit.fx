#version 450

uniform vec4        info;
uniform vec4        info2;
uniform sampler2D   tex0;
uniform sampler2D   tex1;

void main() {
	vec2  uv     = (gl_FragCoord.xy / vec2(512.0, 512.0));
	gl_FragColor = vec4(uv.x);//texture2D(tex0, uv);
}


