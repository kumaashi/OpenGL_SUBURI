#include "ShaderStreams.h"
#include "ShaderSharedStructures.h"
#include "ShaderRandom.h"

#define EXTRUDE_ENABLED
//#define TESSELLATION_ENABLED

float4x4 World : WORLD;
float4x4 WorldView : WORLDVIEW;
float4x4 View : VIEW;
float4x4 ViewInverse : VIEWINVERSE;
float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
float4x4 ViewProjection : VIEWPROJECTION;

StructuredBuffer <float4> DeformedVertexBuffer : DEFORMEDVERTEXBUFFER;
StructuredBuffer <float4> UndeformedVertexBuffer : UNDEFORMEDVERTEXBUFFER;
StructuredBuffer <float4> DeformedNormalBuffer : DEFORMEDNORMALBUFFER;
StructuredBuffer <float2> DeformedUVBuffer : DEFORMEDUVBUFFER;

StructuredBuffer <uint> SourceVertexIndexBuffer : SOURCEVERTEXINDEXBUFFER;
StructuredBuffer <float4x4> CloneInstanceTransformBuffer : CLONETRANSFORMBUFFER;

float MaterialIndex : MATERIALINDEX;
float ObjectIndex : OBJECTINDEX;
float4 MaterialColour : MATERIALCOLOUR;
float4 ExtrudedMaterialColour : EXTRUDEDMATERIALCOLOUR;

float3 CameraPosition : EYEPOSITION;

float4x4 ProceduralTextureTransform : PROCEDURALTEXTURETRANSFORM;
float ProceduralTextureParameter : PROCEDURALTEXTUREPARAMETER;
float ProceduralTextureWeight : PROCEDURALTEXTUREWEIGHT;

Texture2D <float4> MaterialColourTexture : MaterialColourTexture;
Texture2D <float4> NormalMap : NormalMap;


Texture2D <uint> CellOffsetTexture : CELLOFFSETBUFFER;
RWStructuredBuffer <uint> RWPixelCountBuffer : RWPIXELCOUNTBUFFER;
RWStructuredBuffer <float> RWPixelBuffer : RWPIXELBUFFER;
RWStructuredBuffer <uint2> RWNormalDepthPixelBuffer : RWNORMALDEPTHPIXELBUFFER;
uint2 PixelBufferSize : PIXELBUFFERSIZE;

float GrowTime : FERTILIZERTIME;
float FertilizerBackTime : FERTILIZERBACKTIME;
uint VertexBaseIndex : VERTEXBASEINDEX;
float NormalSmoothing : NORMALSMOOTHING;
float MaxTessellationFactor : MAXTESSELLATIONFACTOR;
float WriteOnOffMode : WRITEONOFFMODE;
float ExtrudeAmount : EXTRUDEAMOUNT;
float BevelAmount : BEVELAMOUNT;

sampler PointClampSampler
{
	Filter = Min_Mag_Mip_Point;
    AddressU = Clamp;
    AddressV = Clamp;
};
sampler LinearClampSampler
{
	Filter = Min_Mag_Mip_Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};
sampler LinearWrapSampler
{
	Filter = Min_Mag_Point_Mip_Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VsOutput
{
#ifdef TESSELLATION_ENABLED
	float3 Position		: CPOINT;  
#else
    float4 Position		: SV_POSITION;  
#endif
	float4 Colour		: COLOR0;
	float2 Uv			: TEXCOORD0;
	float3 Normal		: TEXCOORD1;
	float3 LocalPosition : TEXCOORD2;	
	float3 WorldPosition : TEXCOORD3;
#ifdef FERTILIZER
	float FertilizerVertexTime : TEXCOORD4;
#endif	
#ifdef TESSELLATION_ENABLED
	float2 TessFactor : TEXCOORD5;	
#endif
};

struct VsShadowOutput
{
	float4 Position		: SV_POSITION;  
	float3 WorldPosition : TEXCOORD0;
};



struct HsConstantOutput
{
    float Edges[3]        : SV_TessFactor;
    float Inside[1]       : SV_InsideTessFactor;
};
struct HsOutput
{		
	float3 Position		: CPOINT;  
	float4 Colour		: COLOR0;
	float2 Uv			: TEXCOORD0;
	float3 Normal		: TEXCOORD1;
	float3 LocalPosition : TEXCOORD2;
	float3 WorldPosition : TEXCOORD3;
#ifdef FERTILIZER
	float FertilizerVertexTime : TEXCOORD4;
#endif	
	float2 TessFactor : TEXCOORD5;	
	
	float4 PnPatchData0 : TEXCOORD6;
	float3 PnPatchData1 : TEXCOORD7;
	float3 PnPatchData2 : TEXCOORD8;
};

struct PsInput
{
    float4 Position		: SV_POSITION;  
	float4 Colour		: COLOR0;
	float2 Uv			: TEXCOORD0;
	float3 Normal		: TEXCOORD1;
	float3 LocalPosition : TEXCOORD2;	
	float3 WorldPosition : TEXCOORD3;
#ifdef FERTILIZER
	float FertilizerVertexTime : TEXCOORD4;
#endif	
#ifdef TESSELLATION_ENABLED
	float2 TessFactor : TEXCOORD5;	
#endif
};


/*
struct GBufferOutput
{
	float4 ColourObjectId : SV_TARGET0;
	float4 NormalMaterialId : SV_TARGET1;
};
*/

float GetFertilizerClipValue( float t )
{
	if( GrowTime <= t 
	|| FertilizerBackTime > t )
		return -1;
	else 
		return 1;
}
float3 EvaluatePosition(out float3 undeformedPosition, uint vidx)
{
	uint origVertexId = SourceVertexIndexBuffer[vidx];
	undeformedPosition = UndeformedVertexBuffer[origVertexId].xyz;
	return DeformedVertexBuffer[origVertexId].xyz;
}	

float GetGridValueForAxis(float3 pos)
{
	pos = frac(pos + 1000.0f);	
	pos = pos * 2.0f - 1.0f;
	
	float3 ax = saturate(abs(pos));
	ax = -(ax - (1.0f-ProceduralTextureParameter));
	ax *= 1000.0f/ProceduralTextureParameter;
	ax = saturate(ax);
	
	float a = min(ax.x,min(ax.y,ax.z));
	return a;
}

float3 EvaluateProceduralTextureGrid(float3 pos)
{
	float3 procPos = mul(float4(pos,1.0f), ProceduralTextureTransform).xyz;
	float gridValue = GetGridValueForAxis(procPos);
	
	return gridValue * 0.75f + 0.25f;
}


#ifdef USE_GENERATED_SHADER	
float3 GetDistanceFieldNormal(float3 worldPosition)
{
	float eps = 0.001f;
	float3 d0 = float3(EvaluateDistanceFieldLinear(worldPosition + float3(-eps,0,0),0),EvaluateDistanceFieldLinear(worldPosition + float3(0,-eps,0),0),EvaluateDistanceFieldLinear(worldPosition + float3(0,0,-eps),0));
	float3 d1 = float3(EvaluateDistanceFieldLinear(worldPosition + float3( eps,0,0),0),EvaluateDistanceFieldLinear(worldPosition + float3(0, eps,0),0),EvaluateDistanceFieldLinear(worldPosition + float3(0,0, eps),0));
	
	float3 nrm = d1 - d0;
	return -normalize(nrm);
}
float3 GetDistanceFieldIntersection(float3 worldPosition)
{
//	if(BoxDistance(worldPosition) > 0.0f)
//		return worldPosition;

	float3 dir = normalize(worldPosition - CameraPosition);
	
	float3 pos = worldPosition;
	for(uint i = 0; i < 50; ++i)
	{
		float d = EvaluateDistanceFieldLinear(pos,int3(pos*128.0f));
		pos += dir * -d * 1.0f;//0.25f;
	}
	return pos;
}
#endif


#ifdef DEFORMERS_ENABLED
VsOutput VS_Main(uint VertexIdx : SV_VertexID
#else
VsOutput VS_Main(
  float4 Position : POSITION
, float2 Times : TEXCOORD0
, float3 Bevels : TEXCOORD1
, uint VertexIdx : SV_VertexID
#endif
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
{
	VsOutput Out = (VsOutput)0;
	
#ifdef DEFORMERS_ENABLED
	float3 undeformedPosition;
	float3 Position = EvaluatePosition(undeformedPosition, VertexIdx);
#else
	float3 undeformedPosition = Position.xyz;
#endif	
	
	float FertilizerVertexTime = WriteOnOffMode < 0.5f ? Times.x : Times.y;
	float3 normal = float3(0,0,-1.0f);
	float2 Uv = Position.xy * 0.5f + 0.5f;
	
#ifndef DEFORMERS_ENABLED	
	undeformedPosition.xyz += Bevels * BevelAmount;
	Position.xyz += Bevels * BevelAmount;
#endif
	Position.z *= ExtrudeAmount;
//	undeformedPosition.z *= ExtrudeAmount;
	
	float3 worldPosition = mul( float4(Position.xyz,1), World ).xyz;
	float3 worldNormal = normalize(mul(float4(normal,0), World).xyz);
#ifdef INSTANCED_TRANSFORM		
	worldPosition.xyz = mul(float4(worldPosition.xyz, 1.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz;
	worldNormal.xyz = normalize(mul(float4(worldNormal.xyz, 0.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz);
#endif
	
#ifdef TESSELLATION_ENABLED	
	Out.Position = worldPosition;
	Out.TessFactor = MaxTessellationFactor;
#else
	Out.Position = mul( float4(worldPosition.xyz,1), ViewProjection );
#endif
	Out.Normal = worldNormal;
	Out.Uv = Uv;
	Out.Colour = MaterialColour;
	Out.LocalPosition = undeformedPosition;
	
#ifdef FERTILIZER
	Out.FertilizerVertexTime = FertilizerVertexTime;
#endif	

	return Out;
}
float4 PS_Main( PsInput In ) : SV_TARGET
{ 
#ifdef FERTILIZER
	clip(GetFertilizerClipValue(In.FertilizerVertexTime));
#endif

	/*
	float3 normal = In.Normal.xyz;
	float3 lat = normalize(ddx(In.WorldPosition.xyz));
	float3 up = normalize(ddy(In.WorldPosition.xyz));
	float3 faceNormal = normalize(cross(lat,up));
	faceNormal = normalize(mul(float4(faceNormal,0),View).xyz);
	normal = normalize(lerp(faceNormal, normal, saturate(NormalSmoothing)));

	return float4(normal * 0.5f + 0.5f, 1);
	*/

	//float4 textureColour = MaterialColourTexture.Sample(LinearWrapSampler, In.Uv);
	return In.Colour;// * textureColour;
}




#ifdef TESSELLATION_ENABLED

HsConstantOutput HS_ConstantDefault(InputPatch<VsOutput, 3> patch)
{
    HsConstantOutput output;
		
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
	
	float3 viewDirection0 = normalize(CameraPosition - patch[0].Position);
	float3 viewDirection1 = normalize(CameraPosition - patch[1].Position);
	float3 viewDirection2 = normalize(CameraPosition - patch[2].Position); 
	float viewDpMax = max( dot(patch[0].Normal, viewDirection0), max(dot(patch[1].Normal, viewDirection1), dot(patch[2].Normal, viewDirection2)) ) ; 
		
	float2 tessFactor = patch[0].TessFactor;
	// normal factor - causes cracks
	//tessFactor *= normalFactor;
	
	tessFactor = max(tessFactor,1.0f);
	
	// backface cull 
	tessFactor *= viewDpMax >= -0.1f;
			
	float edges = tessFactor.x;
	float inside = tessFactor.y;
	
    output.Edges[0] = edges;
    output.Edges[1] = edges;
	output.Edges[2] = edges;
	output.Inside[0] = inside;

    return output;
}

#define WIJ(i, j) dot(ip[j].Position.xyz - ip[i].Position.xyz, ip[i].Normal.xyz)
#define VIJ(i,j) (2.0f*dot(ip[j].Position.xyz - ip[i].Position.xyz, ip[i].Normal+ip[j].Normal)/dot(ip[j].Position.xyz - ip[i].Position.xyz, ip[j].Position.xyz - ip[i].Position.xyz))

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_ConstantDefault")]
HsOutput HS_Default(InputPatch<VsOutput, 3> ip, uint id : SV_OutputControlPointID)
{
    HsOutput output;
    output.Position = ip[id].Position;
	output.Normal = ip[id].Normal;
	output.Colour = ip[id].Colour;
	output.Uv = ip[id].Uv;
	output.LocalPosition = ip[id].LocalPosition;
	output.WorldPosition = ip[id].WorldPosition;
#ifdef FERTILIZER	
	output.FertilizerVertexTime = ip[id].FertilizerVertexTime;
#endif
	output.TessFactor = ip[id].TessFactor;
	
	// set base 
	float P0 = ip[0].Position[id];
	float P1 = ip[1].Position[id];
	float P2 = ip[2].Position[id];
	float N0 = ip[0].Normal[id];
	float N1 = ip[1].Normal[id];
	float N2 = ip[2].Normal[id];
	
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
PsInput DS_Default(HsConstantOutput input, OutputPatch<HsOutput, 3> op, float3 uv : SV_DomainLocation)
{
    PsInput output;
    
    float3 uvSquared = uv*uv;
	float3 uvCubed   = uvSquared*uv;

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
	float3 n200 = op[0].Normal.xyz;
	float3 n020 = op[1].Normal.xyz;
	float3 n002 = op[2].Normal.xyz;
    
	float3 p = uv.x * op[0].Position + uv.y * op[1].Position + uv.z * op[2].Position;
	float3 n= uv.x * op[0].Normal + uv.y * op[1].Normal + uv.z * op[2].Normal;
	
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
	n = finalNormal;
	
	
    output.Position =  mul(float4(p.xyz,1.0f), ViewProjection);
	output.Normal = normalize(mul(float4(n,0.0f), View).xyz);
	
	output.Uv = uv.x * op[0].Uv + uv.y * op[1].Uv + uv.z * op[2].Uv;
	output.LocalPosition = uv.x * op[0].LocalPosition + uv.y * op[1].LocalPosition + uv.z * op[2].LocalPosition;
	output.WorldPosition = uv.x * op[0].WorldPosition + uv.y * op[1].WorldPosition + uv.z * op[2].WorldPosition;
#ifdef FERTILIZER		
	output.FertilizerVertexTime = uv.x * op[0].FertilizerVertexTime + uv.y * op[1].FertilizerVertexTime + uv.z * op[2].FertilizerVertexTime;
#endif
	output.Colour = uv.x * op[0].Colour + uv.y * op[1].Colour + uv.z * op[2].Colour;

    return output;
}

#endif


#ifdef DEFORMERS_ENABLED
VsOutput VS_GBuffer(
uint VertexIdx : SV_VertexID
#else
VsOutput VS_GBuffer(
float4 Position : POSITION, 
float2 Times : TEXCOORD0,
float3 Bevels : TEXCOORD1,
uint VertexIdx : SV_VertexID
#endif
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
{
	VsOutput Out = (VsOutput)0;
	
	
#ifdef DEFORMERS_ENABLED
	float3 undeformedPosition;
	float3 Position = EvaluatePosition(undeformedPosition, VertexIdx);
#else
	float3 undeformedPosition = Position.xyz;
#endif		
	float FertilizerVertexTime = WriteOnOffMode < 0.5f ? Times.x : Times.y;
	float3 normal = float3(0,0,1.0f);
#ifndef DEFORMERS_ENABLED		
	undeformedPosition.xyz += Bevels * BevelAmount;
	Position.xyz += Bevels * BevelAmount;
#endif
	float2 Uv = Position.xy * 0.5f + 0.5f;
	
#ifdef FERTILIZER
	Out.FertilizerVertexTime = FertilizerVertexTime;
#endif	

	Position.z *= ExtrudeAmount;
	//undeformedPosition.z *= ExtrudeAmount;

	float3 worldPosition = mul( float4(Position.xyz,1), World ).xyz;
	float3 worldNormal = normalize(mul(float4(normal,0), World).xyz);
#ifdef INSTANCED_TRANSFORM		
	worldPosition.xyz = mul(float4(worldPosition.xyz, 1.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz;
	worldNormal.xyz = normalize(mul(float4(worldNormal.xyz, 0.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz);
#endif
	
	Out.WorldPosition = worldPosition;
#ifdef TESSELLATION_ENABLED	
	Out.Position = worldPosition;
	Out.TessFactor = MaxTessellationFactor;
#else	
	Out.Position = mul( float4(worldPosition.xyz,1), ViewProjection );
#endif
	Out.Normal = normalize(mul(float4(worldNormal,0), View).xyz);
	Out.Uv = Uv;
	Out.Colour = MaterialColour;
	Out.LocalPosition = undeformedPosition;

	return Out;
}



#ifdef EXTRUDE_ENABLED

float3 ClampDirToAxis(float3 dir)
{
	if(abs(dir.y) > abs(dir.x))
	{
		return float3(0,sign(dir.y),0);
	}
	else
	{
		return float3(sign(dir.x),0,0);
	}
}

[maxvertexcount(3)] void GS_RenderGeometryDisplacement( triangle VsOutput input[3], inout TriangleStream<VsOutput> OutputStream )
{	
	float3 pos0 = input[0].WorldPosition.xyz;
	float3 pos1 = input[1].WorldPosition.xyz;
	float3 pos2 = input[2].WorldPosition.xyz;

	float3 v0 = pos1 - pos0;
	float3 v1 = pos2 - pos0;
	float3 v = cross(v0,v1);
	
	float3 localNrm = normalize(cross(normalize(input[1].LocalPosition.xyz - input[0].LocalPosition.xyz),normalize(input[2].LocalPosition.xyz - input[0].LocalPosition.xyz)));
	
	float3 centrePos = (pos0+pos1+pos2) * (1.0f/3.0f);
	float bevelAmt = 0.0f;
	
	float3 nrm = normalize(v);
	
	if(localNrm.z < 0.99f)
	{
		input[0].Colour = ExtrudedMaterialColour;
		input[1].Colour = ExtrudedMaterialColour;
		input[2].Colour = ExtrudedMaterialColour;
	}
		
	if(localNrm.z >= 0.99f || ExtrudeAmount > 0.005f)
	{
		OutputStream.Append( input[2] );
		OutputStream.Append( input[1] );
		OutputStream.Append( input[0] );
	}
		
#if 0		
	if(ExtrudeAmount > 0.0001f)
	{
		//float3 nrm = normalize(cross(normalize(v0),normalize(v1)));
		float3 nrm = normalize(mul(float4(input[0].Normal,0), ViewInverse).xyz);
		
		float3 beveled0 = pos0;
		float3 beveled1 = pos1;
		float3 beveled2 = pos2;
			
		float3 bevelDir0 = normalize(pos0 - centrePos); 
		float3 bevelDir1 = normalize(pos1 - centrePos); 
		float3 bevelDir2 = normalize(pos2 - centrePos); 	
		
		pos0 = pos0 + bevelDir0 * BevelAmount;
		pos1 = pos1 + bevelDir1 * BevelAmount;
		pos2 = pos2 + bevelDir2 * BevelAmount;
		
		pos0 += nrm * ExtrudeAmount * BevelAmount * 2.0f;
		pos1 += nrm * ExtrudeAmount * BevelAmount * 2.0f;
		pos2 += nrm * ExtrudeAmount * BevelAmount * 2.0f;
		
		input[0].Position = mul(float4(pos0,1.0f), ViewProjection);
		input[1].Position = mul(float4(pos1,1.0f), ViewProjection);
		input[2].Position = mul(float4(pos2,1.0f), ViewProjection);
				
		float3 extruded0 = pos0 + nrm * ExtrudeAmount;
		float3 extruded1 = pos1 + nrm * ExtrudeAmount;
		float3 extruded2 = pos2 + nrm * ExtrudeAmount;
		
		float4 e0vp = mul(float4(extruded0,1.0f), ViewProjection);
		float4 e1vp = mul(float4(extruded1,1.0f), ViewProjection);
		float4 e2vp = mul(float4(extruded2,1.0f), ViewProjection);
		
		
		float4 b0vp = mul(float4(beveled0,1.0f), ViewProjection);
		float4 b1vp = mul(float4(beveled1,1.0f), ViewProjection);
		float4 b2vp = mul(float4(beveled2,1.0f), ViewProjection);
				
		VsOutput Out = input[0];
		
		Out = input[0];
		Out.WorldPosition = pos0;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = input[0].Position;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = pos1;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = input[1].Position;
		OutputStream.Append( Out );
		
		Out = input[0];
		Out.WorldPosition = extruded0;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e0vp;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = extruded1;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e1vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		Out = input[1];
		Out.WorldPosition = pos1;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = input[1].Position;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = pos2;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = input[2].Position;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = extruded1;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e1vp;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = extruded2;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e2vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		Out = input[2];
		Out.WorldPosition = pos2;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = input[2].Position;
		OutputStream.Append( Out );
		
		Out = input[0];
		Out.WorldPosition = pos0;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = input[0].Position;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = extruded2;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e2vp;
		OutputStream.Append( Out );
		
		Out = input[0];
		Out.WorldPosition = extruded0;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e0vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		Out = input[0];
		Out.WorldPosition = extruded0;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e0vp;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = extruded1;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e1vp;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = extruded2;
		Out.Colour = ExtrudedMaterialColour;
		Out.Position = e2vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		
		if(BevelAmount > 0.0001f)
		{	
			// more damn quads 
			
			
			
			Out = input[0];
			Out.WorldPosition = beveled0;
			Out.Colour = ExtrudedMaterialColour;
			Out.Position = b0vp;
			OutputStream.Append( Out );
		
			Out = input[1];
			Out.WorldPosition = beveled1;
			Out.Colour = ExtrudedMaterialColour;
			Out.Position = b1vp;
			OutputStream.Append( Out );
		
			Out = input[0];
			Out.WorldPosition = pos0;
			Out.Colour = ExtrudedMaterialColour;
			OutputStream.Append( Out );
		
			Out = input[1];
			Out.WorldPosition = pos1;
			Out.Colour = ExtrudedMaterialColour;
			OutputStream.Append( Out );
		
			OutputStream.RestartStrip();
			
			
			Out = input[1];
			Out.WorldPosition = beveled1;
			Out.Colour = ExtrudedMaterialColour;
			Out.Position = b1vp;
			OutputStream.Append( Out );
		
			Out = input[2];
			Out.WorldPosition = beveled2;
			Out.Colour = ExtrudedMaterialColour;
			Out.Position = b2vp;
			OutputStream.Append( Out );
		
			Out = input[1];
			Out.WorldPosition = pos1;
			Out.Colour = ExtrudedMaterialColour;
			OutputStream.Append( Out );
		
			Out = input[2];
			Out.WorldPosition = pos2;
			Out.Colour = ExtrudedMaterialColour;
			OutputStream.Append( Out );
		
			OutputStream.RestartStrip();
		
		
			Out = input[2];
			Out.WorldPosition = beveled2;
			Out.Colour = ExtrudedMaterialColour;
			Out.Position = b2vp;
			OutputStream.Append( Out );
		
			Out = input[0];
			Out.WorldPosition = beveled0;
			Out.Colour = ExtrudedMaterialColour;
			Out.Position = b0vp;
			OutputStream.Append( Out );
		
			Out = input[2];
			Out.WorldPosition = pos2;
			Out.Colour = ExtrudedMaterialColour;
			OutputStream.Append( Out );
		
			Out = input[0];
			Out.WorldPosition = pos0;
			Out.Colour = ExtrudedMaterialColour;
			OutputStream.Append( Out );
		
			OutputStream.RestartStrip();
			
			
			Out = input[0];
			Out.WorldPosition = beveled0;
			Out.Position = b0vp;
			OutputStream.Append( Out );
		
			Out = input[1];
			Out.WorldPosition = beveled1;
			Out.Position = b1vp;
			OutputStream.Append( Out );
		
			Out = input[2];
			Out.WorldPosition = beveled2;
			Out.Position = b2vp;
			OutputStream.Append( Out );
		
			OutputStream.RestartStrip();
			
		}
		else
		{
				
		Out = input[0];
		Out.WorldPosition = pos0;
		Out.Position = input[0].Position;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = pos1;
		Out.Position = input[1].Position;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = pos2;
		Out.Position = input[2].Position;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		}
	}
	else
	{
		OutputStream.Append( input[0] );
		OutputStream.Append( input[1] );
		OutputStream.Append( input[2] );
		
	}
	#endif
}



[maxvertexcount(15)] void GS_RenderGeometryShadowDisplacement( triangle VsShadowOutput input[3], inout TriangleStream<VsShadowOutput> OutputStream )
{	
	float3 pos0 = input[0].WorldPosition.xyz;
	float3 pos1 = input[1].WorldPosition.xyz;
	float3 pos2 = input[2].WorldPosition.xyz;

	float3 v0 = pos1 - pos0;
	float3 v1 = pos2 - pos0;
	float3 v = cross(v0,v1);
	float area = 0.5f * (abs(v.x) + abs(v.y) + abs(v.z));
	
	float3 centrePos = (pos0+pos1+pos2) * (1.0f/3.0f);
	float bevelAmt = 0.1f;
#if 0
			
	if(ExtrudeAmount > 0.001f)
	{
		float3 nrm = normalize(cross(normalize(v0),normalize(v1)));
		
		float3 extruded0 = lerp(pos0,centrePos,bevelAmt) + nrm * ExtrudeAmount;
		float3 extruded1 = lerp(pos1,centrePos,bevelAmt) + nrm * ExtrudeAmount;
		float3 extruded2 = lerp(pos2,centrePos,bevelAmt) + nrm * ExtrudeAmount;
		
		float4 e0vp = mul(float4(extruded0,1.0f), ViewProjection);
		float4 e1vp = mul(float4(extruded1,1.0f), ViewProjection);
		float4 e2vp = mul(float4(extruded2,1.0f), ViewProjection);
		
		VsShadowOutput Out = input[0];
		
		Out = input[0];
		Out.WorldPosition = pos0;
		Out.Position = input[0].Position;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = pos1;
		Out.Position = input[1].Position;
		OutputStream.Append( Out );
		
		Out = input[0];
		Out.WorldPosition = extruded0;
		Out.Position = e0vp;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = extruded1;
		Out.Position = e1vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		Out = input[1];
		Out.WorldPosition = pos1;
		Out.Position = input[1].Position;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = pos2;
		Out.Position = input[2].Position;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = extruded1;
		Out.Position = e1vp;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = extruded2;
		Out.Position = e2vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		Out = input[2];
		Out.WorldPosition = pos2;
		Out.Position = input[2].Position;
		OutputStream.Append( Out );
		
		Out = input[0];
		Out.WorldPosition = pos0;
		Out.Position = input[0].Position;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = extruded2;
		Out.Position = e2vp;
		OutputStream.Append( Out );
		
		Out = input[0];
		Out.WorldPosition = extruded0;
		Out.Position = e0vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		Out = input[0];
		Out.WorldPosition = extruded0;
		Out.Position = e0vp;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = extruded1;
		Out.Position = e1vp;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = extruded2;
		Out.Position = e2vp;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
		
		
		Out = input[0];
		Out.WorldPosition = pos0;
		Out.Position = input[0].Position;
		OutputStream.Append( Out );
		
		Out = input[1];
		Out.WorldPosition = pos1;
		Out.Position = input[1].Position;
		OutputStream.Append( Out );
		
		Out = input[2];
		Out.WorldPosition = pos2;
		Out.Position = input[2].Position;
		OutputStream.Append( Out );
		
		OutputStream.RestartStrip();
	}
	else
	{
		OutputStream.Append( input[0] );
		OutputStream.Append( input[1] );
		OutputStream.Append( input[2] );
	}
#else
	OutputStream.Append( input[0] );
	OutputStream.Append( input[1] );
	OutputStream.Append( input[2] );
#endif
}



#endif // EXTRUDE_ENABLED



float4 PS_RenderPixelCounts( VsOutput In, bool IsFrontFace : SV_IsFrontFace ) : SV_TARGET0
{
#ifdef FERTILIZER
	clip(GetFertilizerClipValue(In.FertilizerVertexTime));
#endif

#ifdef USE_GENERATED_SHADER
	if(IsFrontFace)
	{
		float d = EvaluateDistanceFieldLinear(In.WorldPosition, int3(In.WorldPosition*128.0f));	
		clip(-d);
	}
#endif

	return 1.0f;
}



float4 PS_RenderPixelSamples( VsOutput In, bool IsFrontFace : SV_IsFrontFace ) : SV_TARGET0
{
	bool isVisible = true;
#ifdef FERTILIZER
	float fertilizerValue = GetFertilizerClipValue(In.FertilizerVertexTime);
	clip(fertilizerValue);
	isVisible = fertilizerValue >= 0.0f;
#endif

#ifdef USE_GENERATED_SHADER
	if(IsFrontFace)
	{
		float d = EvaluateDistanceFieldLinear(In.WorldPosition, int3(In.WorldPosition*128.0f));	
		clip(-d);
		if(d > 0.0f)
			isVisible = false;
	}
#endif	

	if(isVisible)
	{	
		float viewDepth = mul(float4(In.WorldPosition,1.0f),View).z;
					
		uint2 pixelPos = In.Position.xy;
		uint cellIndex = pixelPos.y * PixelBufferSize.x + pixelPos.x;
		uint bufferOffset = CellOffsetTexture.Load(int3(pixelPos,0));
			
		int curCount = 0;
		InterlockedAdd(RWPixelCountBuffer[cellIndex], 1, curCount);
		
		float f = abs(viewDepth);
		if(!IsFrontFace)
			f = -f;
	
		RWPixelBuffer[bufferOffset + curCount] = f;		
	}
	return 0.0f;
}


float4 PS_RenderNormalDepthPixelSamples( VsOutput In, bool IsFrontFace : SV_IsFrontFace ) : SV_TARGET0
{
	bool isVisible = true;
#ifdef FERTILIZER
	float fertilizerValue = GetFertilizerClipValue(In.FertilizerVertexTime);
	clip(fertilizerValue);
	isVisible = fertilizerValue >= 0.0f;
#endif

	float3 normal = In.Normal.xyz;
	float3 faceNormal = normalize(cross(normalize(ddx(In.WorldPosition.xyz)),normalize(ddy(In.WorldPosition.xyz))));
	faceNormal = normalize(mul(float4(faceNormal,0),View).xyz);
	normal = normalize(lerp(faceNormal, normal, saturate(NormalSmoothing)));
	
	if(!IsFrontFace)
		normal *= -1.0f;

	if(isVisible)
	{	
		float viewDepth = mul(float4(In.WorldPosition,1.0f),View).z;
					
		uint2 pixelPos = In.Position.xy;
		uint cellIndex = pixelPos.y * PixelBufferSize.x + pixelPos.x;
		uint bufferOffset = CellOffsetTexture.Load(int3(pixelPos,0));
			
		int curCount = 0;
		InterlockedAdd(RWPixelCountBuffer[cellIndex], 1, curCount);
			
		uint normal8 = PackRGBA8(float4(normal * 0.5f + 0.5f, MaterialIndex));
		
		RWNormalDepthPixelBuffer[bufferOffset + curCount] = uint2(asuint(viewDepth), normal8);	
	}
	return 0.0f;
}

GBufferOutput PS_GBuffer( PsInput In, bool IsFrontFace : SV_IsFrontFace )
{ 
#ifdef FERTILIZER
	clip(GetFertilizerClipValue(In.FertilizerVertexTime));
#endif

#ifdef USE_GENERATED_SHADER
	float d = EvaluateDistanceFieldLinear(In.WorldPosition, int3(In.WorldPosition*128.0f));
	clip(d);
#endif

	float3 normal = In.Normal.xyz;
	float3 lat = normalize(ddx(In.WorldPosition.xyz));
	float3 up = normalize(ddy(In.WorldPosition.xyz));
	float3 faceNormal = normalize(cross(lat,up));
	faceNormal = normalize(mul(float4(faceNormal,0),View).xyz);
	normal = normalize(lerp(faceNormal, normal, saturate(NormalSmoothing)));
	
#ifdef NORMAL_MAPPING	
	float4 normalMapValue = NormalMap.Sample(LinearWrapSampler, In.Uv);
	normalMapValue.xyz = normalMapValue.wyx* 2.0f - 1.0f;
	normalMapValue.z = sqrt(1.0f - normalMapValue.x*normalMapValue.x - normalMapValue.y*normalMapValue.y);
	normal = normalize((normalMapValue.x * lat) + (normalMapValue.y * up) + (normalMapValue.z * faceNormal));
	
#endif	
	if(!IsFrontFace)
		normal *= -1.0f;
		
//	float4 textureColour = MaterialColourTexture.SampleLevel(LinearWrapSampler, In.Uv,0);
//	float3 proceduralColour = EvaluateProceduralTextureGrid(In.LocalPosition);
//	textureColour.xyz = lerp(textureColour.xyz, proceduralColour, ProceduralTextureWeight);
	
	float3 outColour = In.Colour.xyz;// * textureColour.xyz;
	
	GBufferOutput Out;
	Out.ColourObjectId = float4(outColour,ObjectIndex);
	Out.NormalMaterialId = float4(normalize(normal.xyz) * 0.5f + 0.5f, MaterialIndex);
	
	return Out;
}



#ifdef DEFORMERS_ENABLED
VsShadowOutput VS_Shadow(uint VertexIdx : SV_VertexID)
#else
VsShadowOutput VS_Shadow(float4 Position : POSITION)
#endif
{
	VsShadowOutput Out = (VsShadowOutput)0;
	
#ifdef DEFORMERS_ENABLED
	float3 Position;
	float3 undeformedPosition;
	Position.xyz = EvaluatePosition(undeformedPosition, VertexIdx);
#endif
	
	Out.Position = mul( float4(Position.xyz,1), WorldViewProjection );
	return Out;
}
float4 PS_Shadow(VsShadowOutput In) : SV_TARGET
{ 
#ifdef USE_GENERATED_SHADER
//	float d = EvaluateDistanceFieldLinear(In.WorldPosition, int3(In.WorldPosition*128.0f));
//	clip(d);
#endif
	return 1.0f;
}





BlendState NoBlend {
  AlphaToCoverageEnable = FALSE;
  BlendEnable[0] = FALSE;
};
BlendState NoColourWrite {
  AlphaToCoverageEnable = FALSE;
  BlendEnable[0] = FALSE;
  RenderTargetWriteMask[0] = 0;
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
DepthStencilState DepthState {
  DepthEnable = TRUE;
  DepthWriteMask = All;
  DepthFunc = Less_Equal;
  StencilEnable = FALSE; 
};
DepthStencilState NoDepthState {
  DepthEnable = FALSE;
  DepthWriteMask = Zero;
  DepthFunc = Less;
  StencilEnable = FALSE; 
};
DepthStencilState DepthTestOnlyState {
  DepthEnable = TRUE;
  DepthWriteMask = Zero;
  DepthFunc = Less;
  StencilEnable = FALSE; 
};
DepthStencilState NoDepthWriteState {
  DepthEnable = FALSE;
  DepthWriteMask = Zero;
  DepthFunc = Less_Equal;
  StencilEnable = FALSE; 
};
DepthStencilState RenderBooleanStencilDepthState 
{
  DepthEnable = TRUE;
  DepthWriteMask = All;
  DepthFunc = Less;
  StencilEnable = TRUE; 
  StencilReadMask = 255;
  StencilWriteMask = 255;
  FrontFaceStencilFail = Zero;
  FrontFaceStencilDepthFail = Zero;
  FrontFaceStencilPass = Zero;
  FrontFaceStencilFunc = Always;
  BackFaceStencilFail = Keep;
  BackFaceStencilDepthFail = Keep;
  BackFaceStencilPass = Incr;
  BackFaceStencilFunc = Always;
};


DepthStencilState RenderBooleanStencilDepthState2
{
  DepthEnable = TRUE;
  DepthWriteMask = 0;
  DepthFunc = Less;
  StencilEnable = TRUE; 
  StencilReadMask = 255;
  StencilWriteMask = 255;
  FrontFaceStencilFail = Zero;
  FrontFaceStencilDepthFail = Zero;
  FrontFaceStencilPass = Incr;
  FrontFaceStencilFunc = Always;
  BackFaceStencilFail = Keep;
  BackFaceStencilDepthFail = Keep;
  BackFaceStencilPass = Incr;
  BackFaceStencilFunc = Always;
};
DepthStencilState RenderBooleanCutStencilDepthState 
{
  DepthEnable = TRUE;
  DepthWriteMask = All;
  DepthFunc = Less_Equal;
  StencilEnable = TRUE; 
  StencilReadMask = 255;
  StencilWriteMask = 0;
  FrontFaceStencilFail = Keep;
  FrontFaceStencilDepthFail = Keep;
  FrontFaceStencilPass = Keep;
  FrontFaceStencilFunc = Not_Equal;
  BackFaceStencilFail = Keep;
  BackFaceStencilDepthFail = Keep;
  BackFaceStencilPass = Keep;
  BackFaceStencilFunc = Not_Equal;
};


DepthStencilState RenderBooleanCutStencilDepthState2
{
  DepthEnable = TRUE;
  DepthWriteMask = All;
  DepthFunc = Less_Equal;
  StencilEnable = TRUE; 
  StencilReadMask = 255;
  StencilWriteMask = 0;
  FrontFaceStencilFail = Keep;
  FrontFaceStencilDepthFail = Keep;
  FrontFaceStencilPass = Keep;
  FrontFaceStencilFunc = Not_Equal;
  BackFaceStencilFail = Keep;
  BackFaceStencilDepthFail = Keep;
  BackFaceStencilPass = Keep;
  BackFaceStencilFunc = Not_Equal;
};


RasterizerState DefaultRasterState 
{
	CullMode = none;
	FillMode = solid;
	DepthBias = 0;
	ScissorEnable = false;
};

RasterizerState CullFrontRasterState 
{
	CullMode = Front;
	FillMode = solid;
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



technique11 ColorNormal
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_1, VS_Main() ) );
#ifdef TESSELLATION_ENABLED
		SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
#endif	
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_4_1, GS_RenderGeometryDisplacement() ) );
#endif		
		SetPixelShader( CompileShader( ps_5_0, PS_Main() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		//SetBlendState( AdditiveBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( DepthState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}



technique11 Alpha
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_1, VS_Main() ) );
#ifdef TESSELLATION_ENABLED
		SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
#endif		
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_4_1, GS_RenderGeometryDisplacement() ) );
#endif		
		SetPixelShader( CompileShader( ps_5_0, PS_Main() ) );
		
		SetBlendState( LinearBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( DepthTestOnlyState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}




technique11 Shadow
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, VS_Shadow() ) );
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_4_0, GS_RenderGeometryShadowDisplacement() ) );
#endif		
		SetPixelShader( CompileShader( ps_4_0, PS_Shadow() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( DepthState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}


#ifdef USE_GENERATED_SHADER

technique11 DeferredGBufferPass
{
	pass FrontPass
	{
		SetVertexShader( CompileShader( vs_5_0, VS_GBuffer() ) );
#ifdef TESSELLATION_ENABLED
		SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
#endif		
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_5_0, GS_RenderGeometryDisplacement() ) );
#endif
		SetPixelShader( CompileShader( ps_5_0, PS_GBuffer() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( DepthState, 0);
		SetRasterizerState( DefaultRasterState );
	}	
}

#else

technique11 DeferredGBufferPass
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_1, VS_GBuffer() ) );
#ifdef TESSELLATION_ENABLED
		SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
#endif			
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_4_1, GS_RenderGeometryDisplacement() ) );
#endif		
		SetPixelShader( CompileShader( ps_5_0, PS_GBuffer() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( DepthState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}

#endif

technique11 RenderPixelCountsPass
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_5_0, VS_GBuffer() ) );
#ifdef TESSELLATION_ENABLED
		SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
#endif		
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_5_0, GS_RenderGeometryDisplacement() ) );
#endif		
		SetPixelShader( CompileShader( ps_5_0, PS_RenderPixelCounts() ) );
		
		SetBlendState( AdditiveBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthWriteState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}

technique11 RenderPixelSamplesPass
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_5_0, VS_GBuffer() ) );
#ifdef TESSELLATION_ENABLED
		SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
#endif			
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_5_0, GS_RenderGeometryDisplacement() ) );
#endif		
		SetPixelShader( CompileShader( ps_5_0, PS_RenderPixelSamples() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthWriteState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}
technique11 RenderNormalDepthSamplesPass
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_5_0, VS_GBuffer() ) );
#ifdef TESSELLATION_ENABLED
		SetHullShader( CompileShader( hs_5_0, HS_Default() ) );
		SetDomainShader( CompileShader( ds_5_0, DS_Default() ) );
#endif			
#ifdef EXTRUDE_ENABLED	
		SetGeometryShader( CompileShader( gs_5_0, GS_RenderGeometryDisplacement() ) );
#endif		
		SetPixelShader( CompileShader( ps_5_0, PS_RenderNormalDepthPixelSamples() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthWriteState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}