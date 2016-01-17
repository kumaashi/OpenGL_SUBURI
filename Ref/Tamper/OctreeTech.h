
//float4x4 World : WORLD;
//float4x4 WorldViewProjection : WORLDVIEWPROJECTION;

//#include "ShaderRandom.h"
#include "TriangleOctreeCapture.h"

float4x4 World : WORLD;
float4x4 WorldView : WORLDVIEW;
float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
float4x4 View : VIEW;
float4x4 ViewInverse : VIEWINVERSE;
float4x4 ViewProjection : VIEWPROJECTION;
float4x4 Projection : PROJECTION;

float3 GridSize : GRIDSIZE;
float3 InvGridSize : INVGRIDSIZE;

int GridWidth : GRIDWIDTH;
int GridHeight : GRIDHEIGHT;
int GridDepth : GRIDDEPTH;

uint2 PixelBufferSize : DEEPPIXELBUFFERSIZE;

float2 CameraBackProjectProperties : CAMERABACKPROJECT;
float2 InverseProjectionScale : INVERSEPROJECTIONSCALE;
float2 ViewportSize : VIEWPORTSIZE;
float2 InvViewportSize : INVVIEWPORTSIZE;
uint ViewportWidth : VIEWPORTWIDTH;
uint ViewportHeight : VIEWPORTHEIGHT;

float4 MaterialColour : MATERIALCOLOUR;
float3 CameraPosition : EYEPOSITION;
float4 BooleanCutColour : BOOLEANCUTCOLOUR;
float NormalSmoothing : NORMALSMOOTHING;
float PreviousFrameFeedback : PREVIOUSFRAMEFEEDBACK;

float PotentialCutoff : POTENTIALCUTOFF;
int OutputPotentials : OUTPUTPOTENTIALS;
float MaxTessellationFactor : MAXTESSELLATIONFACTOR;

int InitialLevelSize : INITIALLEVELSIZE;

StructuredBuffer <float4> VertexPositionBuffer : VERTEXPOSITIONBUFFER;		
StructuredBuffer <float4> VertexNormalBuffer : VERTEXNORMALBUFFER;				
StructuredBuffer <uint> VertexColourBuffer : VERTEXCOLOURBUFFER;		
StructuredBuffer <uint3> IndexBuffer : INDEXBUFFER;				


Texture2D <uint> CellCountTexture : DEEPPIXELCELLCOUNTBUFFER;
Texture2D <uint> CellOffsetTexture : DEEPPIXELCELLOFFSETTEXTURE;
StructuredBuffer <float> SampleBuffer : SAMPLEBUFFER;
Texture2D <float> DepthTexture : DEPTHBUFFER;

Texture3D <float4> PreviousFrameDistanceTexture : PREVIOUSFRAMEDISTANCETEXTURE;

RWStructuredBuffer <uint> RWPixelCountBuffer : RWDEEPPIXELCOUNTBUFFER;
RWStructuredBuffer <uint2> RWNormalDepthPixelBuffer : RWDEEPNORMALDEPTHPIXELBUFFER;
RWStructuredBuffer <uint4> RWOctreeBuffer : RWOCTREEBUFFER;
StructuredBuffer <uint4> OctreeBuffer : OCTREEBUFFER;
StructuredBuffer <uint4> LevelIndexOffsetBuffer : LEVELINDEXOFFSETBUFFER;	// counters 
StructuredBuffer <uint> CounterBuffer : COUNTERBUFFER;	// counters 
RWStructuredBuffer <uint4> RWLevelIndexOffsetBuffer : RWLEVELINDEXOFFSETBUFFER;	// counters 
RWStructuredBuffer <uint> RWDrawCallBuffer : RWDRAWCALLBUFFER;	


sampler PointClampSampler
{
	Filter = Min_Mag_Mip_Point;
    AddressU = Clamp;
    AddressV = Clamp;
};


struct VS_FS_OUTPUT
{
    float4 Position  : SV_POSITION;
};
struct PS_FS_INPUT
{
    float4 Position  : SV_POSITION;
    uint RTIndex : SV_RenderTargetArrayIndex;
};
struct PS_INPUT
{
	float4 Position  : SV_POSITION;
	float3 GridPosition : TEXCOORD0;
	float ViewDepth : TEXCOORD1;
	float3 Normal : TEXCOORD2;
	float4 Colour : TEXCOORD3;
};
struct HS_INPUT
{
	float4 Position  : CPOINT;
	float3 Normal : TEXCOORD0;
	float3 GridPosition : TEXCOORD1;
	float4 Colour : TEXCOORD2;
};

struct HS_CONSTANT_OUTPUT
{
    float Edges[3]        : SV_TessFactor;
    float Inside[1]       : SV_InsideTessFactor;
};
struct HS_OUTPUT
{		
	float3 Position		: CPOINT;  
	float3 Normal		: TEXCOORD0;
	float3 GridPosition : TEXCOORD1;
	float4 Colour : TEXCOORD2;
	float4 PnPatchData0 : TEXCOORD3;
	float3 PnPatchData1 : TEXCOORD4;
	float3 PnPatchData2 : TEXCOORD5;
};

struct GBufferOutput
{
	float4 ColourObjectId : SV_TARGET0;
	float4 NormalMaterialId : SV_TARGET1;
};


static const float2 SlicePositions[3] = 
{
	float2(-1.0f,1.0f),
	float2(-1.0f,-3.0f),
	float2(3.0f,1.0f),
};


VS_FS_OUTPUT VS_Fullscreen(float4 Position : POSITION)
{
	VS_FS_OUTPUT Out = (VS_FS_OUTPUT)0;
	
	Out.Position = float4(Position.xy,0,1);
	
	return Out;
}

VS_FS_OUTPUT VS_RenderSlices(uint VertexIndex : SV_VertexID)
{
 	VS_FS_OUTPUT Out = (VS_FS_OUTPUT)0;
 	//Out.Position = float4(Position.xy,0.0f,1.0f);
 	
 	int sliceIdx = VertexIndex / 3;
	int vidx = VertexIndex % 3;
	
 	Out.Position = float4(SlicePositions[vidx], 0.00001f + (float)sliceIdx, 1.0f);
 	
	return Out;    
}

[maxvertexcount(3)] void GS_RenderSlices( triangle VS_FS_OUTPUT input[3], inout TriangleStream<PS_FS_INPUT> OutputSliceStream )
{		
	PS_FS_INPUT Out0, Out1, Out2;
	Out0.Position = float4(input[0].Position.xy,0,1);
	Out1.Position = float4(input[1].Position.xy,0,1);
	Out2.Position = float4(input[2].Position.xy,0,1);
	Out0.RTIndex = (uint)input[0].Position.z;
	Out1.RTIndex = (uint)input[1].Position.z;
	Out2.RTIndex = (uint)input[2].Position.z;
		
	OutputSliceStream.Append( Out0 );
	OutputSliceStream.Append( Out1 );
	OutputSliceStream.Append( Out2 );
}


struct PS_DISTANCEFIELD_OUTPUT
{
	float4 Distance : SV_TARGET0;
	float4 Colour : SV_TARGET1;
};

PS_DISTANCEFIELD_OUTPUT PS_GenerateDistanceField(PS_FS_INPUT In)
{
	float3 pos = (float3(In.Position.xy, In.RTIndex) * InvGridSize) * 2.0f - 1.0f;
	
	PS_DISTANCEFIELD_OUTPUT Out;
	
#ifdef USE_GENERATED_SHADER

	float4 col = 1.0f;
	float d = EvaluateDistanceFieldLinear(pos, col);
	//return d;
	if(OutputPotentials)
	{
	//	d = -d;//d = -(d - PotentialCutoff);
	}
	
	float prev = PreviousFrameDistanceTexture.Load(int4(In.Position.xy, In.RTIndex,0)).x;
	d = lerp(d,prev,PreviousFrameFeedback);
	
	Out.Distance = d;
	Out.Colour = col;
	
	return Out;
	
#else
	float d = length(pos) - 0.5f;
	
	Out.Distance = d;
	Out.Colour = 1.0f;
#endif
	
	return Out;

}

float EvaluateCutDistance(float3 pos)
{	
#ifdef USE_GENERATED_SHADER
	
	float d = EvaluateDistanceFieldLinear(pos);
	return d;
	
#else
	float d = length(pos) - 0.5f;
	return d;
#endif
}


float3 GetDistanceFieldNormal(float3 worldPosition)
{
	float eps = 0.001f;
	float3 d0 = float3(EvaluateCutDistance(worldPosition + float3(-eps,0,0)),EvaluateCutDistance(worldPosition + float3(0,-eps,0)),EvaluateCutDistance(worldPosition + float3(0,0,-eps)));
	float3 d1 = float3(EvaluateCutDistance(worldPosition + float3( eps,0,0)),EvaluateCutDistance(worldPosition + float3(0, eps,0)),EvaluateCutDistance(worldPosition + float3(0,0, eps)));
	
	float3 nrm = d1 - d0;
	return -normalize(nrm);
}


uint2 EncodeCellPosition(uint3 pos)
{
	return uint2(pos.x | (pos.y << 16), pos.z);
}
uint3 DecodeCellPosition(uint2 pos)
{
	return uint3(pos.x & 0xffff, (pos.x & 0xffff0000) >> 16, pos.y & 0xffff);
}



#ifdef OCTREE_FUNCTIONS_ENABLED

[numthreads(4,4,4)] void CS_GenerateOctreeLevel0(uint3 GroupId : SV_GroupID, 
						uint3 DispatchThreadId : SV_DispatchThreadID, 
						uint3 GroupThreadId : SV_GroupThreadID,
						uint GroupIndex : SV_GroupIndex)
{
	// evaluate cell centre
	float3 pos = (( (float3(DispatchThreadId.xyz)+0.5f) * InvGridSize) ) * 2.0f - 1.0f;
	
#ifdef USE_GENERATED_SHADER	
	float d = EvaluateDistanceFieldLinear(pos);
#else 
	float d = 1.0f;
#endif
	
	uint cellIndex = DispatchThreadId.x + DispatchThreadId.y * InitialLevelSize + DispatchThreadId.z * InitialLevelSize * InitialLevelSize;
	//d = 0;
	
	float epsilon = 3.5f;
	float addEpsilon = (4.0f/512.0f);
	if(abs(d) < (length(InvGridSize) * epsilon + addEpsilon))
	{
		uint idx = RWOctreeBuffer.IncrementCounter();
		// 8 cells per element added
		idx *= 8;
		// counter is offset by level 0
		idx += InitialLevelSize*InitialLevelSize*InitialLevelSize;
		
		RWOctreeBuffer[cellIndex] = uint4(EncodeCellPosition(DispatchThreadId.xyz), asuint(d), idx);
		
		for(uint i = 0; i < 8; ++i)
		{
			uint3 cellOffset = uint3(i & 1, (i & 2) >> 1, (i & 4) >> 2);
						
			RWOctreeBuffer[idx+i] = uint4( EncodeCellPosition((DispatchThreadId.xyz << 1) + cellOffset), asuint(d), 0xffffffff);
		}
	}
	else
	{
		RWOctreeBuffer[cellIndex] = uint4(EncodeCellPosition(DispatchThreadId.xyz), asuint(d), 0xffffffff);
	}
}

[numthreads(64,1,1)] void CS_GenerateOctree(uint3 GroupId : SV_GroupID, 
						uint3 DispatchThreadId : SV_DispatchThreadID, 
						uint3 GroupThreadId : SV_GroupThreadID,
						uint GroupIndex : SV_GroupIndex)
{
	// Read the cell index
	uint levelOffset = LevelIndexOffsetBuffer[0].x;
	uint cellIndex = DispatchThreadId.x + levelOffset;
	
	if(DispatchThreadId.x < LevelIndexOffsetBuffer[0].y)
	{
	
	uint4 cellValue = RWOctreeBuffer[cellIndex];
	uint3 cellPos = DecodeCellPosition(cellValue.xy);
		
	// evaluate cell centre
	float3 pos = (( (float3(cellPos.xyz)+0.5f) * InvGridSize)) * 2.0f - 1.0f;
#ifdef USE_GENERATED_SHADER	
	float d = EvaluateDistanceFieldLinear(pos);
#else 
	float d = 1.0f;
#endif
	//d = 0;

	RWOctreeBuffer[cellIndex].z = asuint(d);
		
	float epsilon = 3.5f;// * 5.0f;
	float addEpsilon = (4.0f/512.0f);
	if(abs(d) < (length(InvGridSize) * epsilon + addEpsilon))
	{
		uint idx = RWOctreeBuffer.IncrementCounter();
		// 8 cells per element added
		idx *= 8;
		// counter is offset by level 0
		idx += LevelIndexOffsetBuffer[0].w;
		
		RWOctreeBuffer[cellIndex].w = idx;
		
		for(uint i = 0; i < 8; ++i)
		{
			uint3 cellOffset = uint3(i & 1, (i & 2) >> 1, (i & 4) >> 2);
			RWOctreeBuffer[idx+i] = uint4(EncodeCellPosition( (cellPos.xyz << 1) + cellOffset ), asuint(d), 0xffffffff);
		}
	}
	
	}
}


[numthreads(64,1,1)] void CS_GenerateOctreeLevelLast(uint3 GroupId : SV_GroupID, 
						uint3 DispatchThreadId : SV_DispatchThreadID, 
						uint3 GroupThreadId : SV_GroupThreadID,
						uint GroupIndex : SV_GroupIndex)
{
	// Read the cell index
	uint levelOffset = LevelIndexOffsetBuffer[0].x;
	uint cellIndex = DispatchThreadId.x + levelOffset;
	
	if(DispatchThreadId.x < LevelIndexOffsetBuffer[0].y)
	{
		uint4 cellValue = RWOctreeBuffer[cellIndex];
		uint3 cellPos = DecodeCellPosition(cellValue.xy);
		
		// evaluate cell centre
		float3 pos = (( (float3(cellPos.xyz)+0.0f) * InvGridSize)) * 2.0f - 1.0f;
		float4 col = 0;
#ifdef USE_GENERATED_SHADER	
		float d = EvaluateDistanceFieldLinear(pos, col);
#else 
		float d = 1.0f;
#endif
		col = saturate(col) * 255.0f;
		uint4 e = (uint4)col;
		
		RWOctreeBuffer[cellIndex].z = asuint(d);
		RWOctreeBuffer[cellIndex].w = e.x | (e.y << 8) | (e.z << 16) | (e.w << 24);
	}
}


[numthreads(1,1,1)] void CS_UpdateOctreeLevelCounters(uint3 GroupId : SV_GroupID, 
						uint3 DispatchThreadId : SV_DispatchThreadID, 
						uint3 GroupThreadId : SV_GroupThreadID,
						uint GroupIndex : SV_GroupIndex)
{
	uint currentCount = CounterBuffer[0];
	uint levelCount = currentCount;			
	levelCount *= 8;
	
	uint previousLevelEnd = LevelIndexOffsetBuffer[0].w;
	if(previousLevelEnd == 0)
	{
		previousLevelEnd = InitialLevelSize*InitialLevelSize*InitialLevelSize;
	}
	
	RWLevelIndexOffsetBuffer[0] = uint4(previousLevelEnd, levelCount, 0, previousLevelEnd + levelCount);
	
	uint threadGroupCount =  levelCount / 64;
	if((threadGroupCount * 64) < levelCount)
		++threadGroupCount;
	
	RWDrawCallBuffer[0] = threadGroupCount;
    RWDrawCallBuffer[1] = 1;
    RWDrawCallBuffer[2] = 1;
}

#endif

/*
float3 GetSampleDensity(uint2 pixelPosition)
{
	uint offset = CellOffsetTexture.Load(uint3(pixelPosition,0));
	int cnt = CellCountTexture.Load(uint3(pixelPosition,0));
	
	uint cellIndex = pixelPosition.y * SampleBufferSize.x + pixelPosition.x;
	
	uint newCount = SampleCountBuffer[cellIndex];
	
	float3 rsltCol = 0;
	//for(uint i = 0;  i < cnt; ++i)
	for(int i = cnt-1;  i >= 0; --i)
	{		
		AlphaSample sampleValue = SampleBuffer[i + offset];
		float4 col = UnpackRGBA8(sampleValue.m_colour);
	//	s = min(s, sampleValue.m_depth);
	
		float viewDepth = sampleValue.m_depth;
	
		float2 screenPos = float2(pixelPosition) * InvViewportSize;
		float3 viewPosition = float3( float2( screenPos.x * 2.0f - 1.0f, -(screenPos.y * 2.0f - 1.0f) ) * InverseProjectionScale * viewDepth, viewDepth );
		float3 worldPosition = mul(float4(viewPosition,1.0f), ViewInverse).xyz;
		float3 gridPosition = mul(float4(worldPosition.xyz,1.0f), WorldInverse).xyz * 0.5f + 0.5f;
		
		//col.xyz = GetLightColour(gridPosition);
		col.xyz = SampleShadowBuffer(worldPosition);
	
		rsltCol.xyz = lerp(rsltCol.xyz, col.xyz, col.w);
	
		//s = float(sampleValue.m_pos.y) / 1024.0f;
		
		//s = col.w;
	}
	return rsltCol;
	//return s  * 0.1f;
}
*/


struct GBufferDepthOutput
{
	float4 ColourObjectId : SV_TARGET0;
	float4 NormalMaterialId : SV_TARGET1;
	float4 Depth : SV_TARGET2;
};
struct DepthOutput
{
	float Depth : SV_DEPTH;
};


float3 GetViewSpacePosition(float2 screenPos, float viewDepth)
{
	//screenPos.y = -screenPos.y;
	return float3( screenPos * InverseProjectionScale * viewDepth, viewDepth );
}

#ifdef USE_BOOLEANS


// Sort and render fragments.
float GenerateFragmentCuts(out float3 cutPosition, uint2 pixelPosition)
{
	uint offset = CellOffsetTexture.Load(uint3(pixelPosition,0));
	int cnt = CellCountTexture.Load(uint3(pixelPosition,0));
	uint cellIndex = pixelPosition.y * PixelBufferSize.x + pixelPosition.x;
	
	cnt = min(cnt,16);
	
	cutPosition = 0;
	
	// index to current pixel.
	float fragmentDepths[32];
	uint fragmentIndices[32];
	
	int i;
	
	for(i = 0; i < cnt && i < 16; ++i)
	{
		fragmentIndices[i] = i;
		fragmentDepths[i] = SampleBuffer[i + offset];
	}
      
    uint N2 = 1;
    while(N2 < cnt)
		N2 <<= 1;
		
	for(i = cnt; i < N2; ++i)
	{
		fragmentIndices[i] = i;
		fragmentDepths[i] = 100000.0f;
	}
    
    // Unoptimized sorting. (Bitonic Sort)
    // loop from Merge( 2 ) to Merge( nCount )
    for( int nMergeSize=2; nMergeSize<=N2; nMergeSize=nMergeSize*2 )
    {
        // Merge( nCount ) requires log2( nCount ) merges. Merge( nCount/2 ) -> Merge( 2 )
        for( int nMergeSubSize=nMergeSize>>1; nMergeSubSize>0; nMergeSubSize=nMergeSubSize>>1 )
        {
            // compare and swap elements
            for( int nElem=0; nElem<N2; ++nElem )
            {
                int nSwapElem = nElem^nMergeSubSize;
                // check to make sure to only swap once
                if( nSwapElem > nElem )
                {
                    // sort in increasing order
                    if ( ( nElem & nMergeSize ) == 0 && abs(fragmentDepths[ fragmentIndices[ nElem ] ]) > abs(fragmentDepths[ fragmentIndices[ nSwapElem ] ]) ) 
                    {
                        int temp = fragmentIndices[ nElem ];
                        fragmentIndices[ nElem ] = fragmentIndices[ nSwapElem ];
                        fragmentIndices[ nSwapElem ] = temp;
                    }

                    // sort in descending order
                    if ( ( nElem & nMergeSize ) != 0 && abs(fragmentDepths[ fragmentIndices[ nElem ] ]) < abs(fragmentDepths[ fragmentIndices[ nSwapElem ] ]) ) 
                    {
                        int temp = fragmentIndices[ nElem ];
                        fragmentIndices[ nElem ] = fragmentIndices[ nSwapElem ];
                        fragmentIndices[ nSwapElem ] = temp;
                    }
                }
            }
        }
    }
    
    float2 screenPos = float2(pixelPosition) * InvViewportSize;
    screenPos = float2( screenPos.x * 2.0f - 1.0f, -(screenPos.y * 2.0f - 1.0f) );
    
    float viewDepthCut = 1000.0f;
    
    for(i = 0; i < cnt - 1; ++i)
    {
		//if(fragmentDepths[ fragmentIndices[i] ] > 0.0f)
		{
			float viewDepth0 = abs(fragmentDepths[ fragmentIndices[i] ]); 
			float3 viewSpacePos0 = float3(screenPos * InverseProjectionScale * viewDepth0, viewDepth0 );
		
			float viewDepth1 = abs(fragmentDepths[ fragmentIndices[i+1] ]); 
			float3 viewSpacePos1 = float3(screenPos * InverseProjectionScale * viewDepth1, viewDepth1 );
							
			float3 pos0 = mul(float4(viewSpacePos0,1.0f),ViewInverse);
			float3 pos1 = mul(float4(viewSpacePos1,1.0f),ViewInverse);
			
			float3 rayDir = normalize(pos0 - CameraPosition);
									
			float d0 = EvaluateCutDistance(pos0);
			if(i == 0 && d0 >= 0)
				return viewDepthCut;
			
			float d1 = EvaluateCutDistance(pos1);	
			
			// we assume d0 is < 0 because it's a clipped out front facing surface. 
						
			float midD = 10000.0f;
			float3 cutPos = pos0;
			for(uint j = 0; j < 20 && abs(midD) > 0.002f; ++j)
			{
				cutPos = (pos0+pos1) * 0.5f;
				midD = EvaluateCutDistance(cutPos);
				
				if(midD >= 0.0f)
				{
					pos1 = cutPos;
				}				
				else
				{
					pos0 = cutPos;
				}
			}					
			float cutDist = midD;
			
			
			
			/*
			uint iters = 50;
			float cutDist = 0.0f;
			float3 cutPos = pos0;
			float p01dist = length(pos1-pos0);
			float pdir = normalize(pos1-pos0);
			bool keepGoing = true;
			for(uint j = 0; j < iters && cutDist < p01dist && keepGoing; ++j)
			{				
				cutPos = pos0 + pdir * cutDist;
				float nd = EvaluateCutDistance(cutPos);
				if(nd >= 0.0f)
				{
					keepGoing = false;
				}
				cutDist += -nd * 0.5f;
			}
			*/
			
			float4 projPosCut = mul(float4(cutPos,1.0f), ViewProjection);
			float depthCut = projPosCut.z / projPosCut.w;
		
			if(abs(cutDist) < 0.005f && depthCut < viewDepthCut)
			//if(!keepGoing && depthCut < viewDepthCut)
			{				
				cutPosition = cutPos;
				viewDepthCut = depthCut;//min(viewDepthCut,depthCut);
				i = cnt;
			}
		}
    }
    return viewDepthCut;
}


GBufferDepthOutput PS_RenderBooleanCuts( VS_FS_OUTPUT In )
{ 
	uint2 pixelPosition = In.Position.xy;
	
	float depth = DepthTexture.Load(int3(In.Position.xy,0));
	
	uint offset = CellOffsetTexture.Load(uint3(pixelPosition,0));
	int cnt = CellCountTexture.Load(uint3(pixelPosition,0));
	uint cellIndex = pixelPosition.y * PixelBufferSize.x + pixelPosition.x;
		
	GBufferDepthOutput Out = (GBufferDepthOutput)0;
	Out.ColourObjectId = float4(0,0,0,ObjectIndex);
	Out.NormalMaterialId = float4(0,0,1,MaterialIndex);
	float3 cutPosition = 0.0f;
	Out.Depth = 1000.0f;
	
	//bool isCut = false;
	
	float cutDepth = 1000.0f;
	if(cnt > 0)
	{
		cutDepth = GenerateFragmentCuts(cutPosition, pixelPosition);	
			
		float3 cutNormal = GetDistanceFieldNormal(cutPosition);
			
		cutNormal = normalize(mul(float4(cutNormal,0), View).xyz);
			
		Out.NormalMaterialId = float4(cutNormal,MaterialIndex);
		Out.ColourObjectId.xyz = BooleanCutColour.xyz;
			
		Out.Depth = cutDepth;	
	}
	
	clip(cutDepth >= depth ? -1.0f : 1.0f);
	
	return Out;
}


DepthOutput PS_OutputDepth( VS_FS_OUTPUT In )
{
	float depth = DepthTexture.Load(int3(In.Position.xy,0));
	DepthOutput Out;
	Out.Depth = depth;
	
	clip(depth < 1.0f ? 1.0f : -1.0f);
	
	//clip(-1);
	
	return Out;
}

#endif



#ifndef USE_BOOLEANS 

PS_INPUT VS_RenderIndexed(uint VertexIndex : SV_VertexID)
{
 	PS_INPUT Out = (PS_INPUT)0;
 	
 	float4 Position = VertexPositionBuffer[VertexIndex];
 	
 	Out.Position = mul(float4(Position.xyz,1.0f), WorldViewProjection);
 	Out.Normal = 0;
 	if(NormalSmoothing > 0.001f)
	{
		Out.Normal = normalize(VertexNormalBuffer[VertexIndex].xyz);
	}
	Out.GridPosition = Position.xyz;
	
	uint vertexColour = VertexColourBuffer[VertexIndex];
	Out.Colour = float4((float)(vertexColour & 0xff), (float)((vertexColour & 0xff00)>>8), (float)((vertexColour & 0xff0000)>>16), (float)((vertexColour & 0xff000000)>>24)) * (1.0f/255.0f);  
	Out.ViewDepth = mul(float4(Position.xyz,1.0f), WorldView).z;
 	 	
	return Out;    
}

HS_INPUT VS_RenderIndexedTessellation(uint VertexIndex : SV_VertexID)
{
 	HS_INPUT Out = (HS_INPUT)0;
 	
 	float4 Position = VertexPositionBuffer[VertexIndex];
 	
 	Out.Position = mul(float4(Position.xyz,1.0f), WorldView);
 	Out.Normal = 0;
 	if(NormalSmoothing > 0.001f)
	{
		Out.Normal = normalize(mul(float4(normalize(VertexNormalBuffer[VertexIndex].xyz),0.0f),WorldView));
	}
	Out.GridPosition = Position.xyz;
	
	uint vertexColour = VertexColourBuffer[VertexIndex];
	Out.Colour = float4((float)(vertexColour & 0xff), (float)((vertexColour & 0xff00)>>8), (float)((vertexColour & 0xff0000)>>16), (float)((vertexColour & 0xff000000)>>24)) * (1.0f/255.0f);  
		 	
	return Out;    
}

HS_CONSTANT_OUTPUT HS_ConstantDefault(InputPatch<HS_INPUT, 3> patch)
{
    HS_CONSTANT_OUTPUT output;
		
	float3 v0 = patch[1].Position - patch[0].Position;
	float3 v1 = patch[2].Position - patch[0].Position;
	float3 crossV = cross(v0,v1);
	float len = abs(crossV.x) + abs(crossV.y) + abs(crossV.z);
	float triArea = len * 0.5f;
		
	float3 faceNormal = normalize(cross(normalize(v0),normalize(v1)));
	float dp0 = abs(dot(faceNormal, patch[0].Normal));
	float dp1 = abs(dot(faceNormal, patch[1].Normal));
	float dp2 = abs(dot(faceNormal, patch[2].Normal));
	float minDp = min(dp0, min(dp1,dp2));
	minDp = saturate((minDp - 0.8f) * 5.0f);
	minDp = pow(minDp,8.0f);		
	float normalFactor = saturate(1.0f - minDp);
	
	float3 viewDirection0 = normalize(-patch[0].Position);
	float3 viewDirection1 = normalize(-patch[1].Position);
	float3 viewDirection2 = normalize(-patch[2].Position); 
	float viewDpMax = max( dot(patch[0].Normal, viewDirection0), max(dot(patch[1].Normal, viewDirection1), dot(patch[2].Normal, viewDirection2)) ) ; 
		
	float2 tessFactor = MaxTessellationFactor;
	// normal factor - causes cracks
	//tessFactor *= normalFactor;
	
	tessFactor = max(tessFactor,1.0f);
	
	// backface cull 
	//tessFactor *= viewDpMax >= -0.1f;
			
	float edges = tessFactor.x;
	float inside = tessFactor.y;
	
    output.Edges[0] = edges;
    output.Edges[1] = edges;
	output.Edges[2] = edges;
	output.Inside[0] = inside;

    return output;
}

#define WIJ(i, j) dot(ip[j].Position.xyz - ip[i].Position.xyz, ip[i].Normal.xyz)
#define VIJ(i,j) (2.0f*dot(ip[j].Position.xyz - ip[i].Position.xyz, normals[i]+normals[j])/dot(ip[j].Position.xyz - ip[i].Position.xyz, ip[j].Position.xyz - ip[i].Position.xyz))

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_ConstantDefault")]
HS_OUTPUT HS_Default(InputPatch<HS_INPUT, 3> ip, uint id : SV_OutputControlPointID)
{
    HS_OUTPUT output;
    output.Position = ip[id].Position;
	output.Normal = ip[id].Normal;
	output.Colour = ip[id].Colour;
	output.GridPosition = ip[id].GridPosition;
	
	// set base 
	float P0 = ip[0].Position[id];
	float P1 = ip[1].Position[id];
	float P2 = ip[2].Position[id];
	
	float3 normals[3];
	
	normals[0] = ip[0].Normal;
	normals[1] = ip[1].Normal;
	normals[2] = ip[2].Normal;
	
	float3 faceNormal = normalize(cross(normalize(ip[1].Position.xyz - ip[0].Position.xyz),normalize(ip[2].Position.xyz - ip[0].Position.xyz)));
	if(NormalSmoothing > 0.001f)
	{
		normals[0] = normalize(lerp(faceNormal,normals[0],NormalSmoothing));
		normals[1] = normalize(lerp(faceNormal,normals[1],NormalSmoothing));
		normals[2] = normalize(lerp(faceNormal,normals[2],NormalSmoothing));
	}
	else
	{
		normals[0] = faceNormal;
		normals[1] = faceNormal;
		normals[2] = faceNormal;
	}
	
	
	float N0 = normals[0][id];
	float N1 = normals[1][id];
	float N2 = normals[2][id];
		
	// compute control points (will be evaluated three times ...)
	float b210 = (2.0f*P0 + P1 - WIJ(0,1)*N0)/3.0f;
	float b120 = (2.0f*P1 + P0 - WIJ(1,0)*N1)/3.0f;
	float b021 = (2.0f*P1 + P2 - WIJ(1,2)*N1)/3.0f;
	float b012 = (2.0f*P2 + P1 - WIJ(2,1)*N2)/3.0f;
	float b102 = (2.0f*P2 + P0 - WIJ(2,0)*N2)/3.0f;
	float b201 = (2.0f*P0 + P2 - WIJ(0,2)*N0)/3.0f;
	
	float E = ( b210
	          + b120
	          + b021
	          + b012
	          + b102
	          + b201 ) / 6.0f;
	float V = (P0 + P1 + P2)/3.0f;
	float b111 = E + (E - V)*0.5;
	float n110 = N0+N1-VIJ(0,1)*(P1-P0);
	float n011 = N1+N2-VIJ(1,2)*(P2-P1);
	float n101 = N2+N0-VIJ(2,0)*(P0-P2);
	
	output.PnPatchData0 = float4(b210, b120, b021, b012);
	output.PnPatchData1 = float3(b102, b201, b111);
	output.PnPatchData2 = float3(n110, n011, n101);

    return output;
}

[domain("tri")]
PS_INPUT DS_Default(HS_CONSTANT_OUTPUT input, OutputPatch<HS_OUTPUT, 3> op, float3 uv : SV_DomainLocation)
{
    PS_INPUT output;
    
    float3 uvSquared = uv*uv;
	float3 uvCubed   = uvSquared*uv;
	
	float3 normals[3];	
	normals[0] = op[0].Normal;
	normals[1] = op[1].Normal;
	normals[2] = op[2].Normal;
	
	float3 faceNormal = normalize(cross(normalize(op[1].Position.xyz - op[0].Position.xyz),normalize(op[2].Position.xyz - op[0].Position.xyz)));
	if(NormalSmoothing > 0.001f)
	{
		normals[0] = normalize(lerp(faceNormal,op[0].Normal,NormalSmoothing));
		normals[1] = normalize(lerp(faceNormal,op[1].Normal,NormalSmoothing));
		normals[2] = normalize(lerp(faceNormal,op[2].Normal,NormalSmoothing));
	}
	else
	{
		normals[0] = faceNormal;
		normals[1] = faceNormal;
		normals[2] = faceNormal;
	}
	

	// extract control points
	float3 b210 = float3(op[0].PnPatchData0.x, op[1].PnPatchData0.x, op[2].PnPatchData0.x);
	float3 b120 = float3(op[0].PnPatchData0.y, op[1].PnPatchData0.y, op[2].PnPatchData0.y);
	float3 b021 = float3(op[0].PnPatchData0.z, op[1].PnPatchData0.z, op[2].PnPatchData0.z);
	float3 b012 = float3(op[0].PnPatchData0.w, op[1].PnPatchData0.w, op[2].PnPatchData0.w);
	float3 b102 = float3(op[0].PnPatchData1.x, op[1].PnPatchData1.x, op[2].PnPatchData1.x);
	float3 b201 = float3(op[0].PnPatchData1.y, op[1].PnPatchData1.y, op[2].PnPatchData1.y);
	float3 b111 = float3(op[0].PnPatchData1.z, op[1].PnPatchData1.z, op[2].PnPatchData1.z);
	float3 b300 = op[0].Position.xyz;
	float3 b030 = op[1].Position.xyz;
	float3 b003 = op[2].Position.xyz;

	// extract control normals
	float3 n110 = normalize(float3(op[0].PnPatchData2.x,
	                           op[1].PnPatchData2.x,
	                           op[2].PnPatchData2.x));
	float3 n011 = normalize(float3(op[0].PnPatchData2.y,
	                           op[1].PnPatchData2.y,
	                           op[2].PnPatchData2.y));
	float3 n101 = normalize(float3(op[0].PnPatchData2.z,
	                           op[1].PnPatchData2.z,
	                           op[2].PnPatchData2.z));
	float3 n200 = normals[0].xyz;
	float3 n020 = normals[1].xyz;
	float3 n002 = normals[2].xyz;
    
	float3 p = uv.x * op[0].Position + uv.y * op[1].Position + uv.z * op[2].Position;
	float3 n= uv.x * normals[0] + uv.y * normals[1] + uv.z * normals[2];
	
	float uTessAlpha = 1.0f;
	
	// normal
	float3 barNormal = uv.z*op[0].Normal + uv.x*op[1].Normal + uv.y*op[2].Normal;
	               
	float3 pnNormal  = n200*uvSquared.z
	               + n020*uvSquared.x
	               + n002*uvSquared.y
	               + n110*uv.z*uv.x
	               + n011*uv.x*uv.y
	               + n101*uv.z*uv.y;
	float3 finalNormal = uTessAlpha*pnNormal + (1.0-uTessAlpha)*barNormal; // should we normalize ?

	// compute interpolated pos
	float3 barPos = uv.z*b300
	            + uv.x*b030
	            + uv.y*b003;

	// save some computations
	uvSquared *= 3.0f;

	// compute PN position
	float3 pnPos  = b300*uvCubed.z
	            + b030*uvCubed.x
	            + b003*uvCubed.y
	            + b210*uvSquared.z*uv.x
	            + b120*uvSquared.x*uv.z
	            + b201*uvSquared.z*uv.y
	            + b021*uvSquared.x*uv.y
	            + b102*uvSquared.y*uv.z
	            + b012*uvSquared.y*uv.x
	            + b111*6.0f*uv.x*uv.y*uv.z;

	// final position and normal
	float3 finalPos = (1.0-uTessAlpha)*barPos + uTessAlpha*pnPos;
	
	p = finalPos;
	n = normalize(finalNormal);
	
	
    output.Position =  mul(float4(p.xyz,1.0f), Projection);
	output.Normal = n;
	output.GridPosition = uv.x * op[0].GridPosition + uv.y * op[1].GridPosition + uv.z * op[2].GridPosition;
	output.ViewDepth = p.z;
	output.Colour = uv.x * op[0].Colour + uv.y * op[1].Colour + uv.z * op[2].Colour;
	
    return output;
}





[maxvertexcount(3)] void GS_RenderMarchingCubes( triangle PS_INPUT input[3], inout TriangleStream<PS_INPUT> Output )
{		
	PS_INPUT Out0, Out1, Out2;
	Out0 = input[0];
	Out1 = input[1];
	Out2 = input[2];
	
	float3 faceNormal = normalize(cross(normalize(Out1.GridPosition - Out0.GridPosition),normalize(Out2.GridPosition - Out0.GridPosition)));
	
	if(NormalSmoothing > 0.001f)
	{
		Out0.Normal = normalize(lerp(faceNormal,Out0.Normal,NormalSmoothing));
		Out1.Normal = normalize(lerp(faceNormal,Out1.Normal,NormalSmoothing));
		Out2.Normal = normalize(lerp(faceNormal,Out2.Normal,NormalSmoothing));
	}
	else
	{
		Out0.Normal = faceNormal;
		Out1.Normal = faceNormal;
		Out2.Normal = faceNormal;
	}
	Out0.Normal = normalize(mul(float4(Out0.Normal,0), WorldView).xyz  );
	Out1.Normal = normalize(mul(float4(Out1.Normal,0), WorldView).xyz  );
	Out2.Normal = normalize(mul(float4(Out2.Normal,0), WorldView).xyz  );
	
	float3 centrePos = (Out0.GridPosition + Out1.GridPosition + Out2.GridPosition) * (1.0f/3.0f);
	Out0.GridPosition = centrePos;
	Out1.GridPosition = centrePos;
	Out2.GridPosition = centrePos;
	
	Out0.Colour = input[0].Colour;
	Out1.Colour = input[1].Colour;
	Out2.Colour = input[2].Colour;
		
	Output.Append( Out0 );
	Output.Append( Out1 );
	Output.Append( Out2 );
	
	
}

GBufferOutput PS_RenderMarchingCubes(PS_INPUT In)
{
	/*
	float3 nrm = 0;
	if(NormalSmoothing > 0.001f)
	{
		nrm = normalize(mul(float4(In.Normal,0), WorldView).xyz  );
	}
	float3 flatNrm = normalize( mul(float4(cross(ddx(In.GridPosition.xyz), ddy(In.GridPosition.xyz)), 0.0f), WorldView).xyz  );
	nrm = normalize(lerp(flatNrm,nrm,NormalSmoothing));
	*/
	float3 nrm = In.Normal;
	
	float4 col = In.Colour;
#ifdef USE_GENERATED_SHADER
	//EvaluateDistanceFieldLinear(float3(In.GridPosition.x,In.GridPosition.y,In.GridPosition.z)*2.0f-1.0f, In.GridPosition*128.0f, col);
#endif
	
//	nrm = EvaluateSdfNormal(In.GridPosition.xyz);
//	nrm = -nrm;
	
	GBufferOutput Out;
	Out.ColourObjectId = float4(MaterialColour.xyz * col.xyz,ObjectIndex);
	Out.NormalMaterialId = float4(nrm * 0.5f + 0.5f, MaterialIndex);
	
	return Out;
}


float4 PS_RenderShadow(PS_INPUT In) : SV_TARGET0
{
	return 1.0f;
}



float4 PS_RenderPixelCounts( PS_INPUT In ) : SV_TARGET0
{
	return 1.0f;
}


uint PackNormal8(float4 c)
{
	float4 fc = saturate(c) * 255.0f;
	uint4 e = (uint4)fc;

	return e.x | (e.y << 8) | (e.z << 16) | (e.w << 24);
}
		
			

float4 PS_RenderNormalDepthPixelSamples( PS_INPUT In ) : SV_TARGET0
{
	float3 normal = normalize(In.Normal);//mul(float4(In.Normal,0),View).xyz);
	float viewDepth = In.ViewDepth;
			
	uint2 pixelPos = In.Position.xy;
	uint cellIndex = pixelPos.y * PixelBufferSize.x + pixelPos.x;
	uint bufferOffset = CellOffsetTexture.Load(int3(pixelPos,0));
			
	int curCount = 0;
	InterlockedAdd(RWPixelCountBuffer[cellIndex], 1, curCount);
			
				
	uint normal8 = PackNormal8(float4(normal * 0.5f + 0.5f, MaterialIndex));
		
	RWNormalDepthPixelBuffer[bufferOffset + curCount] = uint2(asuint(viewDepth), normal8);	
	
	return 0.0f;
}


VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT VS_CaptureTriangleOctreeGeometry(uint VertexIndex : SV_VertexID)
{
	VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT Out = (VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT)0;
		
	float4 Position = VertexPositionBuffer[VertexIndex];
	uint vertexColour = VertexColourBuffer[VertexIndex];
	Out.Colour = float4((float)(vertexColour & 0xff), (float)((vertexColour & 0xff00)>>8), (float)((vertexColour & 0xff0000)>>16), (float)((vertexColour & 0xff000000)>>24)) * (1.0f/255.0f);  
	
	float3 worldPosition = mul( float4(Position.xyz,1), World ).xyz;	
	Out.Position = mul( float4(worldPosition.xyz,1), ViewProjection );
	Out.Uv = 0;
	Out.Normal = 0;
 	if(NormalSmoothing > 0.001f)
	{
		Out.Normal = normalize(VertexNormalBuffer[VertexIndex].xyz);
		Out.Normal = normalize(mul( float4(Out.Normal.xyz,0), World ).xyz);	
		Out.Normal = normalize(mul( float4(Out.Normal.xyz,0), ViewProjection ).xyz);	
	}
	
	return Out;	
}


#endif



BlendState NoBlend {
  AlphaToCoverageEnable = FALSE;
  BlendEnable[0] = FALSE;
};
   
DepthStencilState NoDepthState {
  DepthEnable = FALSE;
  DepthWriteMask = All;
  DepthFunc = Less;
  StencilEnable = FALSE; 
};

BlendState AdditiveBlend 
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend[0] = ONE;
	DestBlend[0] = ONE;
	BlendOp[0] = ADD;
	SrcBlendAlpha[0] = ONE;
	DestBlendAlpha[0] = ZERO;
	BlendOpAlpha[0] = ADD;
	
	BlendEnable[1] = TRUE;
	SrcBlend[1] = ONE;
	DestBlend[1] = ONE;
	BlendOp[1] = ADD;
	SrcBlendAlpha[1] = ONE;
	DestBlendAlpha[1] = ZERO;
	BlendOpAlpha[1] = ADD;
};  
DepthStencilState TestDepthState {
  DepthEnable = TRUE;
  DepthWriteMask = All;
  DepthFunc = Less;
  StencilEnable = FALSE; 
};
DepthStencilState NoDepthWriteState {
  DepthEnable = TRUE;
  DepthWriteMask = Zero;
  DepthFunc = Less_Equal;
  StencilEnable = FALSE; 
};

RasterizerState DefaultRasterState 
{
	CullMode = None;
	FillMode = Solid;
	DepthBias = 0;
	ScissorEnable = false;
};

RasterizerState CullBackRasterState 
{
	CullMode = Back;
	FillMode = solid;
	DepthBias = 0;
	ScissorEnable = false;
};
technique11 GenerateDistanceField
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_4_0, VS_RenderSlices() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS_RenderSlices() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_GenerateDistanceField() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }
};


#ifdef USE_BOOLEANS

technique11 RenderBooleanCuts
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_5_0, VS_Fullscreen() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_RenderBooleanCuts() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }
};


technique11 OutputBooleanDepth
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_5_0, VS_Fullscreen() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_OutputDepth() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( TestDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }
};

#endif



#ifndef USE_BOOLEANS


technique11 RenderIndexedGeometry
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_4_0, VS_RenderIndexed() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS_RenderMarchingCubes() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_RenderMarchingCubes() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( TestDepthState, 0);
		SetRasterizerState( CullBackRasterState );
    }    
};

technique11 RenderIndexedGeometryTessellation
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_5_0, VS_RenderIndexedTessellation() ) );
        SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
        //SetGeometryShader( CompileShader( gs_5_0, GS_RenderMarchingCubes() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_RenderMarchingCubes() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( TestDepthState, 0);
		SetRasterizerState( CullBackRasterState );
    }    
};


technique11 RenderShadowIndexedGeometry
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_4_0, VS_RenderIndexed() ) );
        SetPixelShader( CompileShader( ps_4_0, PS_RenderShadow() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( TestDepthState, 0);
		SetRasterizerState( CullBackRasterState );
    }    
};


technique11 RenderPixelCountsPass
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_1, VS_RenderIndexed() ) );
        SetGeometryShader( CompileShader( gs_4_1, GS_RenderMarchingCubes() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_RenderPixelCounts() ) );
		
		SetBlendState( AdditiveBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthWriteState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}


technique11 RenderNormalDepthSamplesPass
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_1, VS_RenderIndexed() ) );
        SetGeometryShader( CompileShader( gs_4_1, GS_RenderMarchingCubes() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_RenderNormalDepthPixelSamples() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthWriteState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}

technique11 CaptureTriangleOctreeGeometry
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_5_0, VS_CaptureTriangleOctreeGeometry() ) );
        SetGeometryShader( CompileShader( gs_5_0, GS_CaptureTriangleOctreeGeometry() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_CaptureTriangleOctreeGeometry() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }    
}


#ifdef OCTREE_FUNCTIONS_ENABLED

technique11 GenerateOctreeLevel0
{
	pass p0
	{
		SetComputeShader( CompileShader( cs_5_0, CS_GenerateOctreeLevel0() ) );
	}
}

technique11 GenerateOctree
{
	pass p0
	{
		SetComputeShader( CompileShader( cs_5_0, CS_GenerateOctree() ) );
	}
}

technique11 GenerateOctreeLevelLast
{
	pass p0
	{
		SetComputeShader( CompileShader( cs_5_0, CS_GenerateOctreeLevelLast() ) );
	}
}

technique11 UpdateOctreeLevelCounters
{
	pass p0
	{
		SetComputeShader( CompileShader( cs_5_0, CS_UpdateOctreeLevelCounters() ) );
	}
}

#endif // OCTREE_FUNCTIONS_ENABLED

#endif
