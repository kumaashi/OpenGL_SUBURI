#ifdef GL_ES
precision mediump float;
#endif

uniform vec4        info;
uniform vec4        info2;
uniform sampler2D   tex;
#define AO_REP      4
#define SAMPLES     8
#define PI          3.14159265358979323846
#define ADSAMPLES   ((2 * PI) / SAMPLES)
#define RADIUS      0.003
#define RADIUS_MULT 1.03
#define STRONG      1

float GetDepth(float d) {
	float zNear = info.z;
	float zFar  = info.w;
	return (-zFar * zNear / (d * (zFar - zNear) - zFar));
}

float getao(vec2 uv) {
	vec4  col    = texture2D(tex, uv);
	float depth  = GetDepth(col.w);
	float radius = RADIUS;
	float g      = 1.0;
	float mult   = 1.0;
	float angle  = 0.0;
	if(depth >= (info.w * 0.99999)) return 1.0;

	for(int k = 0 ; k < AO_REP; k++) {
		for(int i = 0; i < SAMPLES; i++) {
			angle = i * ADSAMPLES;
			vec2 duv = vec2(cos(angle) * radius, sin(angle) * radius);
			vec4  tex  = texture2D(tex, uv + duv);
			float temp = 1.0 - (GetDepth(tex.w) / depth);
			g -= clamp(temp, 0.0, (1.0 / SAMPLES / AO_REP)) * mult;
			radius *= RADIUS_MULT;
		}
		mult   *= 0.9;
	}
	return pow(g, STRONG);
}

void main() {
	vec2  uv     = (gl_FragCoord.xy / info.xy);
	vec4  col    = texture2D(tex, uv);
	vec3  L      = normalize(vec3(1, 2, 3));
	float D      = max(0.0, dot(col.xyz, L));
	float AO     = getao(uv);
	gl_FragColor = texture2D(tex, uv) * D * vec4(AO);
}


