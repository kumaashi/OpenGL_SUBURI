#version 430

uniform float roll;
writeonly uniform image2D destTex;

layout (local_size_x = 32, local_size_y = 32) in;

float map(vec3 p) {
	return length(mod(p, 2.0) - 1.0) - 0.5;
}

void main() {
	ivec2 storePos  = ivec2(gl_GlobalInvocationID.xy);
	float tx        = float(gl_GlobalInvocationID.x);
	float ty        = float(gl_GlobalInvocationID.y);
	float x         = -1.0 + (2.0 * tx / float(640));
	float y         = -1.0 + (2.0 * ty / float(480));
	vec2  uv        = vec2(x, y);
	vec3 dir        = normalize(vec3(uv, 1.0));
	vec3 pos        = vec3(0.0, 0.0, roll);
	float t         = 0.0;
	for(int i = 0 ; i < 100; i++) {
		float temp = map(t * dir + pos);
		if(abs(temp) < 0.001) break;
		t += temp * 0.5;
	}
	imageStore(destTex, storePos, vec4(x, y - t * 0.3, t * 0.1, 1.0));
}

