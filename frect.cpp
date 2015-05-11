#ifdef GL_ES
precision mediump float;
#endif

uniform vec4        info;
uniform vec4        info2;
uniform sampler2D   tex;
#define AO_REP      1
#define SAMPLES     32
#define PI          3.14159265358979323846
#define ADSAMPLES   ((2 * PI) / SAMPLES)
#define EPS         0.50
#define MPS         0.03
#define STRONG      9.2
#define RADIUS      0.003
#define RADIUS_MULT 1.0001

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
	float temp   = 0.0;
	float mult   = 1.0;

	if(depth >= (info.w * 0.99999)) return 1.0;

	for(int k = 0 ; k < AO_REP; k++) {
		for(int i = 0; i < SAMPLES; i++) {
			float angle = i * ADSAMPLES;
			vec2 duv = vec2(cos(angle) * radius, sin(angle) * radius);
			vec4  tex  = texture2D(tex, uv + duv);
			float temp = 1.0 - GetDepth(tex.w) / depth;
			if(abs(temp) > 0.05) continue;
			g -= clamp(temp, 0.0, (1.0 / SAMPLES / AO_REP)) * mult;
			radius *= RADIUS_MULT;
		}
		mult *= 0.7;
	}
	return pow(g, STRONG);
}

void main() {
	vec2  uv     = (gl_FragCoord.xy / info.xy);
	vec4  col    = texture2DLod(tex, uv, 0.0);
	vec2  duv    = vec2(0.0005, 0.0);
	col         += texture2DLod(tex, uv + duv.xy, 0.0);
	col         += texture2DLod(tex, uv + duv.yx, 0.0);
	col         += texture2DLod(tex, uv - duv.xy, 0.0);
	col         += texture2DLod(tex, uv - duv.yx, 0.0);
	col         /= 5.0;

	vec3  L      = normalize(vec3(1, 2, 3));
	float D      = max(0.0, dot(col.xyz, L));
	float AO     = getao(uv);// * D;
	//float AO     = D;
	gl_FragColor = vec4(AO);
}


