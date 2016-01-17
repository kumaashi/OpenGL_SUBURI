
//float4x4 World : WORLD;
//float4x4 WorldViewProjection : WORLDVIEWPROJECTION;

#include "ShaderRandom.h"
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

int InitialLevelSize : INITIALLEVELSIZE;

float MinimumDistanceThreshold : MINIMUMDISTANCETHRESHOLD;

Texture2D <float> DepthTexture : DEPTHBUFFER;
Texture2D <float> PreRayTarget : PRERAYTARGET;
Texture3D <float4> PreviousFrameDistanceTexture : PREVIOUSFRAMEDISTANCETEXTURE;

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

#ifdef USE_GENERATED_SHADER
float3 GetDistanceFieldNormal(float3 worldPosition)
{
	float eps = 0.001f;
	float3 d0 = float3(EvaluateDistanceFieldLinear(worldPosition + float3(-eps,0,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0,-eps,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0,0,-eps)));
	float3 d1 = float3(EvaluateDistanceFieldLinear(worldPosition + float3( eps,0,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0, eps,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0,0, eps)));
	
	float3 nrm = d1 - d0;
	return -normalize(nrm);
}
#else
float3 GetDistanceFieldNormal(float3 worldPosition)
{
	return float3(0,1,0);
}
#endif


struct PS_GBUFFER_OUTPUT
{
	float4 ColourObjectId : SV_TARGET0;
	float4 NormalMaterialId : SV_TARGET1;
};

float3 GetViewSpacePosition(float2 screenPos, float viewDepth)
{
	//screenPos.y = -screenPos.y;
	return float3( screenPos * InverseProjectionScale * viewDepth, viewDepth );
}


void CreateRay(out float3 rayPosition, out float3 rayDirection, uint2 pixelPosition)
{
	float2 screenPos = float2(pixelPosition) * InvViewportSize;
	screenPos = float2( screenPos.x * 2.0f - 1.0f, -(screenPos.y * 2.0f - 1.0f) );
	float3 viewRayDir = normalize(float3(screenPos*InverseProjectionScale,1.0f));
	rayPosition = mul(float4(0,0,0, 1.0f), ViewInverse);
	rayDirection = normalize(mul(float4(viewRayDir, 0.0f), ViewInverse).xyz);
}



float4 PS_EvaluatePreRay( VS_FS_OUTPUT In ) : SV_TARGET0
{
	float3 rayPosition, rayDirection;
	CreateRay(rayPosition, rayDirection, In.Position.xy * 8 + 4);
	
	float totalDistance = 0;
	float3 origPosition = rayPosition;
	
	float3 viewRayDirection = normalize(mul(float4(rayDirection, 0.0f), View).xyz);
	float vrz = viewRayDirection.z;
	float vz = 0;
	
	static const uint exitIterationCount = 1000000;
	uint i = 0;
	[loop] for(i = 0; i < 128; ++i)
	{
		float4 col = 0;
#ifdef USE_GENERATED_SHADER	
		float d = EvaluateDistanceFieldLinear(rayPosition, col);
#else 
		float d = length(rayPosition) - 0.5f;
#endif
		
		d *= 0.5f;
		rayPosition += rayDirection * d;
		
		[flatten] if(abs(d) < 0.05f * vz)
		{
			i = exitIterationCount;
		}	
		else
		{
			totalDistance += d;
		
		}
		vz += vrz * d;
	}
	
	//return totalDistance ;//max(totalDistance,0) + 0.2f;
	return length(rayPosition - origPosition);
}


PS_GBUFFER_OUTPUT PS_RayMarchGBuffer( VS_FS_OUTPUT In )
{ 
	PS_GBUFFER_OUTPUT Out = (PS_GBUFFER_OUTPUT)0;
	uint2 pixelPosition = In.Position.xy;
	
	float preRayDepth = PreRayTarget.Load(uint3(pixelPosition / 8,0));
	
	
	float depth = DepthTexture.Load(int3(In.Position.xy,0));
	float3 rayPosition, rayDirection;
	CreateRay(rayPosition, rayDirection, In.Position.xy);
	
	rayPosition += rayDirection * preRayDepth;
		
	static const uint exitIterationCount = 1000000;
	uint i = 0;
	[loop] for(i = 0; i < 128; ++i)
	{
		float4 col = 0;
#ifdef USE_GENERATED_SHADER	
		float d = EvaluateDistanceFieldLinear(rayPosition, col);
#else 
		float d = length(rayPosition) - 0.5f;
#endif
		
		rayPosition += rayDirection * d * 0.5f;
		[flatten] if(abs(d) < MinimumDistanceThreshold)
		{
			i = exitIterationCount;
		}			
	}
	
	
	// discard invalid fragments
	clip(i >= exitIterationCount ? 1.0f : -1.0f);
		
	float3 normal = GetDistanceFieldNormal(rayPosition);
		
	
	Out.ColourObjectId = float4(1,1,1,ObjectIndex);
	Out.NormalMaterialId = float4(normal * 0.5f + 0.5f,MaterialIndex);
		
	return Out;
}


float EvaluateShadow(float3 rayPosition, float3 lightDir)
{
	float shad = 0;
	for(uint i = 0; i < 16; ++i)
	{
		
#ifdef USE_GENERATED_SHADER	
		float4 c;
		float d = EvaluateDistanceFieldLinear(rayPosition, c);
#else 
		float d = length(rayPosition) - 0.5f;
#endif
		//d = abs(d);
		
		//float n = FbmRotated(rayPosition*3.0f);//FbmHashNoise(rayPosition * 2.0f,6);
		float n = FbmHashNoise(rayPosition * 3.1f,4);
		
		d += n * 2.0f;
		//d *= 0.8f;
		float4 col = lerp(float4(1, 1, 1, 1), float4(0, 0, 0, 0), smoothstep(1.0, 1.1, d));
		
		col.a *= 0.2f;
		
		shad += col.a;
		
		/*
		// pre-multiply alpha
		col.rgb *= col.a;
		colRslt = colRslt + col*(1.0 - colRslt.a);	
		*/
		rayPosition += lightDir * 0.04f;		
	}
	return saturate(shad * 1.0f);
}


float4 PS_RayMarchAlpha( VS_FS_OUTPUT In ) : SV_TARGET0
{ 
	uint2 pixelPosition = In.Position.xy;
	
	float preRayDepth = PreRayTarget.Load(uint3(pixelPosition/8,0));
	preRayDepth *= 0.03f;
	//return float4(preRayDepth,preRayDepth,preRayDepth,1.0f);
	
	float depth = DepthTexture.Load(int3(In.Position.xy,0));
	float3 rayPosition, rayDirection;
	CreateRay(rayPosition, rayDirection, In.Position.xy);
	
	//return float4(	screenPos,0,1);
	//return float4(	abs(rayDirection),1);
	float3 lightDir = float3(0,1,0);
		
		
	static const uint exitIterationCount = 1000000;
	uint i = 0;
	[loop] for(i = 0; i < 32; ++i)
	{
		float4 col = 0;
#ifdef USE_GENERATED_SHADER	
		float d = EvaluateDistanceFieldLinear(rayPosition, col);
#else 
		float d = length(rayPosition) - 0.5f;
#endif
		
		rayPosition += rayDirection * d * 0.5f;			
	}
	rayPosition -= rayDirection * 0.4f;
	
	float4 colRslt = 0.0f;
	for(uint i = 0; i < 16; ++i)
	{
#ifdef USE_GENERATED_SHADER	
		float4 c;
		float d = EvaluateDistanceFieldLinear(rayPosition, c);
#else 
		float d = length(rayPosition) - 0.5f;
#endif
		//d = abs(d);
		
		//float n = FbmRotated(rayPosition*3.0f);//FbmHashNoise(rayPosition * 2.0f,6);
		float n = FbmHashNoise(rayPosition * 3.1f,4);
		
		d += n * 2.0f;
		//d *= 0.8f;
		float4 col = lerp(float4(1, 1, 1, 1), float4(0, 0, 0, 0), smoothstep(1.0, 1.1, d));
		
		col.a *= 0.2f;
		
	//	float s = EvaluateShadow(rayPosition, lightDir);
	//	col.rgb = s;
		
		// pre-multiply alpha
		col.rgb *= col.a;
		colRslt = colRslt + col*(1.0 - colRslt.a);	
		
		rayPosition += rayDirection * 0.04f;			
	}
	return colRslt;// * colRslt.w;
	
	clip(i >= exitIterationCount ? 1.0f : -1.0f);
//	return 1.0f;
		
	float3 normal = GetDistanceFieldNormal(rayPosition);
	return float4(normal * 0.5f + 0.5f, 1.0f);
		
}



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


BlendState LinearBlend 
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend[0] = SRC_ALPHA;
	DestBlend[0] = INV_SRC_ALPHA;
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


technique11 RayMarchGBuffer
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_5_0, VS_Fullscreen() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_RayMarchGBuffer() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }
};

technique11 EvaluatePreRay
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_5_0, VS_Fullscreen() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_EvaluatePreRay() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }
};


technique11 RayMarchAlpha
{
    pass p0
    {   
        SetVertexShader( CompileShader( vs_5_0, VS_Fullscreen() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_RayMarchAlpha() ) );
		
		SetBlendState( LinearBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }
};
