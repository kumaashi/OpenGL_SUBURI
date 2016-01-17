
CompactCellsInLevel0CompactCellsInLevelCompactCellsInLevelLastDistance
Scale		closestObjectColour += float4(%s.xyz * %s.xyz,1.0f) * max(dist,0) * max(dist,0) * %s * %s.w * 10.0f;
		closestObjectColour += float4(%s.xyz,1.0f) * max(dist,0) * max(dist,0) * %s * %s.w;
		if(abs(dist) < abs(closestObjectDistance)) { 
				closestObjectColour = %s * %s; closestObjectDistance = dist;
 }
		if(abs(dist) < abs(closestObjectDistance)) { 
				closestObjectColour = %s; closestObjectDistance = dist;
 }
		sdfValue = sdfValue - dist* %s;
		sdfValue += dist * %s;
		sdfValue = dist;
		sdfValue = min(sdfValue, dist);
		sdfValue = max(sdfValue, dist);
		sdfValue = max(sdfValue, -dist);
		sdfValue = lerp(sdfValue, dist, %s);
	{
		float oldSdfValue = sdfValue;
		float3 oldPos = pos;
		pos = mul(float4(pos, 1.0f), %s);
		float dist = sdfValue;
		sdfValue = oldSdfValue;
		pos = oldPos;
	}
		pos = fmod(pos + 100.0f, %s) - 0.5f * %s;
			pos = mul(float4(pos,1.0f), %s);
		for(int i = 1; i < %s; ++i)
		{
		}
		float3 localPos = mul(float4(pos, 1.0f), %s) * 0.5f + 0.5f;
		float3 pp0 = abs(localPos-0.5) - 0.5;
		float d = max(pp0.x,max(pp0.y,pp0.z));
			float p = %d;
			float3 ppa = fmod(3*p*localPos, 3);			float3 pp = 0.5 - abs(ppa - 1.5);
			pp = mul(float4(pp,1),%s);
			float d1 = min(max(pp.x,pp.z),min(max(pp.x, pp.y),max(pp.y, pp.z))) / (p*3);
			d = max(d,d1);
		float dist = d * 2.0f * %s.w;
		float3 localPos = mul(float4(pos, 1.0f), %s);
		float3 dvec = max(localPos - 1.0f, -1.0f - localPos);
		float dist = max(max(dvec.x,dvec.y),dvec.z);
		float dh = max(localPos.y - 1.0f, -1.0f - localPos.y);
		float dp = localPos.x*localPos.x + localPos.z*localPos.z;
		float dr = sqrt(dp) - 1.0f;
		float dist = max(dr,dh);
		float dist = length(localPos) - 1.0f;
		float dist = dot(localPos, float3(0,1,0));
		dist *= %s.w;
		dist = max(dist * DistanceScale, 0.0f)+0.0001f;
		float sd = 1.0f/(dist);
		dist = sd;
		dist = abs(dist) - %s;
		float3 boxPos = saturate(localPos);
		float dist = %s.SampleLevel(LinearFilterSampler, float3(boxPos.x,1.0f-boxPos.y,boxPos.z), 0);
		float4 sdfColour = %s.SampleLevel(LinearFilterSampler, float3(boxPos.x,1.0f-boxPos.y,boxPos.z), 0);
		dist += length(boxPos-localPos);
		dist += %s;
sdfColour		float dist = 0.0f;
;		float dist = 1.0f;
		float3 samplePos = 	float3(originalInputPos.x, originalInputPos.y, originalInputPos.z) * 0.5f + 0.5f;
		float3 gridWorldPos = pos;
		uint brickSize = 8;
		uint brickMapSize = %s.x / brickSize;
		uint brickOctreeCellSize = ((8*8*8) + (4*4*4) + (2*2*2) + 1);
		int3 brickPos = clamp(int3(samplePos * float(brickMapSize)),0,brickMapSize-1);
		int3 brickLocalPos = int3(samplePos * float(brickMapSize * brickSize)) & (brickSize-1);
		int brickMapIndex = brickPos.x + brickPos.y * brickMapSize + brickPos.z * brickMapSize * brickMapSize;
		int brickIndex = %s[brickMapIndex];
		if(brickIndex != 0xffffffff) {
			int brickSampleAddress = brickIndex * brickOctreeCellSize;
			static const uint octreeLevelOffsetLookup[] = {0, (8*8*8), (8*8*8)+(4*4*4), (8*8*8)+(4*4*4)+(2*2*2) };
			uint3 octreeLevelBrickPos = brickLocalPos >> min(OctreeLevelOffsetFromLeaf, 3);
			uint brickLevelSize = brickSize >> min(OctreeLevelOffsetFromLeaf, 3);
			brickSampleAddress += octreeLevelOffsetLookup[min(OctreeLevelOffsetFromLeaf,3)];
			brickSampleAddress += octreeLevelBrickPos.x + octreeLevelBrickPos.y * brickLevelSize + octreeLevelBrickPos.z * brickLevelSize * brickLevelSize;
			brickSampleAddress += brickLocalPos.x + brickLocalPos.y * brickSize + brickLocalPos.z * brickSize * brickSize;
			dist = %s[brickSampleAddress];
		};
		float4 particleCol = 1.0f;
particleCol		float3 localPos = mul(float4(pos, 1.0f), %s) * 0.5 + 0.5;
		float4 heightmapVal = %s.SampleLevel(LinearFilterSampler,float2(localPos.x,1-localPos.y),0);
		float maxH = heightmapVal.x + %s;
		float dh = max(heightmapVal.x - localPos.z, localPos.z - maxH);
		float coneRadh = abs(dh) * abs(heightmapVal.y);
		dh += (1.0f-heightmapVal.w) * 2.0f;
		float dist = min(dh, coneRadh);
	if(%s) {
		float4 centreParticlePos = %s[%s];
		if(centreParticlePos.w > 0.0f) {
		for(uint i = 1; i < %s; ++i) {
			float4 particlePos = %s[i];
				float3 dir = normalize(particlePos.xyz - centreParticlePos.xyz);
				float3 centrePos = (particlePos.xyz + centreParticlePos.xyz) * 0.5f;
				float planeDistance = dot(pos.xyz - centrePos, dir);
				[flatten] if(particlePos.w > 0.0f) 
					sdfValue = max(sdfValue, planeDistance);
			}
		} else sdfValue = 10000.0f;
DummyColour%dDummy%d%sCSGBlendWeight%sTransformfloat4x4 %s : %sTRANSFORM;
float %s : %sBLENDWEIGHT;
float4 %s : %sCOLOUR;
RepeatColour%d%sRepeatCountfloat3 %s : %sITERTRANSFORM;
ClonerColour%dCloner%d%sIterTransformClonerCount%dfloat4x4 %s : %sITERTRANSFORM;
int %s : %sCLONERCOUNT;
Primitive%d%sTransformScalePrimitiveColour%dPrimitiveShellThickness%dfloat4 %s : %sTRANSFORMSCALE;
float %s : %sSHELLTHICKNESS;
PrimitiveEnabled%duint %s : %sENABLED;
Fractal%dFractalColour%dFractalEnabled%dHeightmap%d%sThicknessHeightmapColour%d%sTexturetexture %s : %sTEXTURE;
float %s : %sTHICKNESS;
HeightmapEnabled%dSDF%d%sColourTexture%sShellThicknessSDFColour%dSDFEnabled%dTexture3D <float> %s : %sTEXTURE;
Particle%d%sBufferParticleColour%d%sColourBuffer%sGridSize%sPotentialCutoff%sParticleSizeStructuredBuffer <uint> %s : %sBUFFER;
StructuredBuffer <float> %s : %sCOLOURBUFFER;
int3 %s : %sBUFFERSIZE;
float %s : %sPOTENTIALCUTOFF;
float %s : %sPARTICLESIZE;
ParticleEnabled%dVoronoi%d%sNumParticles%sParticleIndexStructuredBuffer <float4> %s : %sBUFFER;
uint %s : %sBUFFERSIZE;
uint %s : %sPARTICLEINDEX;
VoronoiEnabled%dfloat EvaluateDistanceFieldCubic(float3 originalInputPos, inout float4 closestObjectColour)
	float3 pos = mul(float4(originalInputPos.xyz* 0.5 + 0.5,1.0f),GridToWorldTransform).xyz;
float EvaluateDistanceFieldLinear(float3 originalInputPos, inout float4 closestObjectColour)
	return sdfValue;
#define OCTREE_FUNCTIONS_ENABLED
float DistanceScale : DISTANCESCALE;
uint OctreeLevelOffsetFromLeaf : OCTREELEVELOFFSETFROMLEAF;
GeneratedMarchingCubesShaderDISTANCESCALE;
		