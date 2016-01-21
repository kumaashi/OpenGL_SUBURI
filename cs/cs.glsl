#version 430

uniform float roll;

//layout (binding=0, rgba32f) writeonly uniform image2D destTex;
layout (binding = 0, rgba32f) uniform image2D destTex;
layout (binding = 1, rgba32i) uniform iimage2D infoTex;

layout (local_size_x = 32, local_size_y = 32) in;

float map(vec3 p) {
	return length(mod(p, 2.0) - 1.0) - 0.5;
}

// in uvec3 gl_NumWorkGroups;
// in uvec3 gl_WorkGroupID;
// in uvec3 gl_LocalInvocationID;
// in uvec3 gl_GlobalInvocationID; //gl_WorkGroupID * gl_WorkGroupSize + gl_LocalInvocationID;
// in uint  gl_LocalInvocationIndex;

void main() {
	ivec2 storePos  = ivec2(gl_GlobalInvocationID.xy);
	/*
	if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0)
	{
		imageStore(destTex, storePos, vec4(1,1,0,1));
		return;
	}
	*/
	uvec3 gi   = gl_GlobalInvocationID;
	ivec3 ipos = ivec3(gi.x, gi.y, gi.z);
	//imageStore(infoTex, storePos, ivec4(ipos, 1) );
	//imageStore(infoTex, storePos, ivec4(1,1,1,1));
	float tx        = float(gl_GlobalInvocationID.x);
	float ty        = float(gl_GlobalInvocationID.y);
	float x         = -1.0 + (2.0 * tx / float(640));
	float y         = -1.0 + (2.0 * ty / float(480));
	vec2  uv        = vec2(x, y);
	/*
	imageStore(destTex, storePos, vec4(tx / float(640),ty / float(480),0,1));
	return;
	*/
	vec3 dir        = normalize(vec3(uv, 1.0));
	vec3 pos        = vec3(0.0, 0.0, roll);
	float t         = 0.0;
	for(int i = 0 ; i < 100; i++) {
		float temp = map(t * dir + pos);
		if(abs(temp) < 0.001) break;
		t += temp * 0.5;
	}
	
	//READ
	vec4 fb = imageLoad(destTex, storePos);
	
	//WRITE
	imageStore(destTex, storePos, 0.9 * fb + 0.1 * vec4(x, y - t * 0.3, t * 0.1, 1.0));
	ivec4 ib = imageLoad(infoTex, storePos);
	if(mod(ib.x, 2) == 0) {
		
		imageStore(destTex, storePos, vec4(ib.x * 0.1));
	}
}

