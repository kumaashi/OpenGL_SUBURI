#ifdef GL_ES
precision mediump float;
#endif

uniform vec4        info;
uniform vec4        info2;
uniform sampler2D   tex;
#define AO_REP      32
#define SAMPLES     8
#define PI          3.14159265358979323846
#define ADSAMPLES   ((2 * PI) / SAMPLES)
#define RADIUS      0.005
#define RADIUS_MULT 1.0003
#define AO_ATTE     0.99
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
		mult   *= AO_ATTE;
	}
	return pow(g, STRONG);
}

float getao2(vec2 uv) {
	vec4  col    = texture2D(tex, uv);
	float depth  = GetDepth(col.w);
	float radius = RADIUS * 0.25;
	float g      = 0.0;
	float mult   = 1.0;
	float angle  = 0.0;
	for(int i = 0; i < SAMPLES; i++) {
		angle = i * ADSAMPLES;
		vec2 duv = vec2(cos(angle) * radius, sin(angle) * radius);
		vec4  tex  = texture2D(tex, uv + duv);
		float temp = 1.0 - (GetDepth(tex.w) / depth);
		if(temp > 0.00001 && temp < 0.01) {
			g += 1.0;
		}
	}
	g = 1.0 - (g / SAMPLES);
	return g;
}

void main() {
	vec2  uv     = (gl_FragCoord.xy / info.xy);
	vec4  col    = texture2D(tex, uv);
	vec3  L      = normalize(vec3(1, 2, 3));
	float D      = max(0.0, dot(col.xyz, L));
	float AO     = getao2(uv);
	gl_FragColor = texture2D(tex, uv) * D * vec4(AO);
	//gl_FragColor = vec4(AO);
}


