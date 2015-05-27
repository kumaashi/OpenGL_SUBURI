#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D   tex;
uniform vec4        info;
uniform vec4        info2;
void main() {
	vec2  uv     = (gl_FragCoord.xy / info.xy);
	gl_FragColor = texture2D(tex, uv);
}

