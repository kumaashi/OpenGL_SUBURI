#ifndef TRIANGLE_OCTREE_CAPTURE_H
#define TRIANGLE_OCTREE_CAPTURE_H

#include "TriangleOctreeDefs.h"

uint SceneTriangleIndexOffset : SCENETRIANGLEINDEXOFFSET;
float MaterialIndex : MATERIALINDEX;
float ObjectIndex : OBJECTINDEX;
AppendStructuredBuffer <TriangleData> RWSceneTriangleBuffer : RWSCENETRIANGLEBUFFER;



struct VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT
{
    float4 Position  : SV_POSITION;
    float2 Uv		: TEXCOORD0;
	float4 Colour : TEXCOORD1;
	float3 Normal : TEXCOORD2;
};
struct PS_TRIANGLE_OCTREE_CAPTURE_INPUT
{
    float4 Position : SV_POSITION;     // Projection coord
    float3 V0 : TEXCOORD0;       // Triangle position 0
    float3 V1 : TEXCOORD1;       // Triangle position 1
    float3 V2 : TEXCOORD2;       // Triangle position 2
    float4 Uvs01 : TEXCOORD3;
    float2 Uv2 : TEXCOORD4;
	float3 Normal0 : TEXCOORD5;
	float3 Normal1 : TEXCOORD6;
	float3 Normal2 : TEXCOORD7;

    uint PrimitiveId : SV_PRIMITIVEID;
};


/*
VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT VS_CaptureTriangleOctreeGeometry(float4 Position : POSITION, float2 Uv : TEXCOORD0)
{
 	VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT Out = (VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT)0;
 	float3 worldPos = mul(float4(Position.xyz,1.0f), World).xyz;
 	Out.Position = mul(float4(worldPos,1.0f), WorldToGridTransform);	
 	Out.Uv = Uv;
	Out.Colour = 1.0f;
	return Out;    
}
*/

[maxvertexcount(1)] void GS_CaptureTriangleOctreeGeometry(
	triangle VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT input[3],
	inout PointStream<PS_TRIANGLE_OCTREE_CAPTURE_INPUT> OutputSliceStream,
	uint TriangleIndex : SV_PRIMITIVEID )
{
	PS_TRIANGLE_OCTREE_CAPTURE_INPUT Out;
	Out.V0 = input[0].Position.xyz;
	Out.V1 = input[1].Position.xyz;
	Out.V2 = input[2].Position.xyz;
	Out.Uvs01 = float4(input[0].Uv,input[1].Uv);
	Out.Uv2 = input[2].Uv;
	Out.Normal0 = input[0].Normal.xyz * 0.5f + 0.5f;
	Out.Normal1 = input[1].Normal.xyz * 0.5f + 0.5f;
	Out.Normal2 = input[2].Normal.xyz * 0.5f + 0.5f;
	uint idx = TriangleIndex + SceneTriangleIndexOffset; 
	uint2 pixelPosition = uint2(idx, idx >> 7) & 127;
	
	// the pixel position is irrelevant - it's just to avoid pixel contention
	float2 pos = (float2(pixelPosition) / 128.0f) * 2.0f - 1.0f;
	pos = pos * 0.8f;
		
	Out.Position = float4(pos,0.0f,1.0f);
	Out.PrimitiveId = TriangleIndex;
	OutputSliceStream.Append( Out );		
}


float4 PS_CaptureTriangleOctreeGeometry(PS_TRIANGLE_OCTREE_CAPTURE_INPUT In) : SV_TARGET0
{
	//uint triangleIndex = In.PrimitiveId + SceneTriangleIndexOffset;
	
	TriangleData tri = (TriangleData)0;
	tri.m_v0 = In.V0;
	tri.m_v1 = In.V1;
	tri.m_v2 = In.V2;
	tri.m_uv0 = In.Uvs01.xy;
	tri.m_uv1 = In.Uvs01.zw;
	tri.m_uv2 = In.Uv2.xy;
	tri.m_materialIndex = MaterialIndex;
	tri.m_objectIndex = ObjectIndex;
	tri.m_colour0 = 0xffffffff;
	tri.m_colour1 = 0xffffffff;
	tri.m_colour2 = 0xffffffff;

	uint3 n0 = uint3(In.Normal0 * 255.0f);
	uint3 n1 = uint3(In.Normal1 * 255.0f);
	uint3 n2 = uint3(In.Normal2 * 255.0f);
	tri.m_normal0 = n0.x | (n0.y << 8) | (n0.z << 16);
	tri.m_normal1 = n1.x | (n1.y << 8) | (n1.z << 16);
	tri.m_normal2 = n2.x | (n2.y << 8) | (n2.z << 16);

	//RWSceneTriangleBuffer[triangleIndex] = tri;
	//uint tidx = RWSceneTriangleBuffer.IncrementCounter();
	//RWSceneTriangleBuffer[tidx] = tri;
	RWSceneTriangleBuffer.Append(tri);
	
	return 0.0f;
}

#endif
