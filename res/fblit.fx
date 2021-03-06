#version 450

uniform vec4        info;
uniform vec4        info2;
uniform sampler2D   tex0;
uniform sampler2D   tex1;
uniform sampler2D   tex2;
uniform sampler2D   tex3;

void main() {
	//vec2  uv     = (gl_FragCoord.xy / vec2(512.0, 512.0));
	//vec2  uv     = (gl_FragCoord.xy / info.xy);
	vec2  uv     = (gl_FragCoord.xy / info2.xy);
	vec4  col    = vec4(0.0);
	col += texture2D(tex0, uv);
	//col += texture2D(tex1, uv);
	col += texture2D(tex2, uv);
	//col += texture2D(tex3, uv);
	/*
	*/
	/*
	*/
	gl_FragColor = vec4(col.xyz, 1.0);
}


