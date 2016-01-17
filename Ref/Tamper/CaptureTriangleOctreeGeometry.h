
#include "ShaderStreams.h"
#include "ShaderSharedStructures.h"
#include "ShaderRandom.h"
#include "TriangleOctreeCapture.h"

//#define GEOMETRY_DISPLACEMENT
//#define TESSELLATION_ENABLED

float4x4 World : WORLD;
float4x4 WorldView : WORLDVIEW;
float4x4 View : VIEW;
float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
float4x4 ViewProjection : VIEWPROJECTION;

StructuredBuffer <float4> DeformedVertexBuffer : DEFORMEDVERTEXBUFFER;
StructuredBuffer <float4> UndeformedVertexBuffer : UNDEFORMEDVERTEXBUFFER;
#ifdef DEFORMERS_ENABLED
StructuredBuffer <float4> DeformedNormalBuffer : DEFORMEDNORMALBUFFER;
#else
StructuredBuffer <uint> DeformedNormalBuffer : DEFORMEDNORMALBUFFER;
#endif
StructuredBuffer <float2> DeformedUVBuffer : DEFORMEDUVBUFFER;
StructuredBuffer <uint> DeformedColourBuffer : DEFORMEDCOLOURBUFFER;


StructuredBuffer <uint> SourceVertexIndexBuffer : SOURCEVERTEXINDEXBUFFER;
StructuredBuffer <uint> IndexBuffer : INDEXBUFFER;

StructuredBuffer <float4x4> CloneInstanceTransformBuffer : CLONETRANSFORMBUFFER;

//float MaterialIndex : MATERIALINDEX;
//float ObjectIndex : OBJECTINDEX;
float4 MaterialColour : MATERIALCOLOUR;
float MaterialLuminosity : MATERIALLUMINOSITY;
float MaterialDiffuse : MATERIALDIFFUSE;

float3 CameraPosition : EYEPOSITION;

float4x4 ProceduralTextureTransform : PROCEDURALTEXTURETRANSFORM;
float ProceduralTextureParameter : PROCEDURALTEXTUREPARAMETER;
float ProceduralTextureWeight : PROCEDURALTEXTUREWEIGHT;

Texture2D <float4> MaterialColourTexture : MaterialColourTexture;
Texture2D <float4> NormalMap : NormalMap;


Texture2D <uint> CellOffsetTexture : DEEPPIXELCELLOFFSETTEXTURE;
RWStructuredBuffer <uint> RWPixelCountBuffer : RWDEEPPIXELCOUNTBUFFER;
RWStructuredBuffer <float> RWPixelBuffer : RWDEEPPIXELBUFFER;
RWStructuredBuffer <uint2> RWNormalDepthPixelBuffer : RWDEEPNORMALDEPTHPIXELBUFFER;
uint2 PixelBufferSize : DEEPPIXELBUFFERSIZE;

uint NumTriangles : NUMTRIANGLES;
uint NumTrianglesPerInstance : NUMTRIANGLESPERINSTANCE;
uint InstanceCount : INSTANCECOUNT;

float GrowTime : FERTILIZERTIME;
float FertilizerBackTime : FERTILIZERBACKTIME;
uint VertexBaseIndex : VERTEXBASEINDEX;
float NormalSmoothing : NORMALSMOOTHING;
float MaxTessellationFactor : MAXTESSELLATIONFACTOR;

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
sampler AnisoWrapSampler
{
	Filter = Anisotropic;
    MaxAnisotropy = 8;
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
	float3 d0 = float3(EvaluateDistanceFieldLinear(worldPosition + float3(-eps,0,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0,-eps,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0,0,-eps)));
	float3 d1 = float3(EvaluateDistanceFieldLinear(worldPosition + float3( eps,0,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0, eps,0)),EvaluateDistanceFieldLinear(worldPosition + float3(0,0, eps)));
	
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
		float d = EvaluateDistanceFieldLinear(pos);
		pos += dir * -d * 1.0f;//0.25f;
	}
	return pos;
}
#endif


#ifdef DEFORMERS_ENABLED
VsOutput VS_Main(
uint VertexIdx : SV_VertexID
#ifdef INSTANCED_TRANSFORM
, uint InstanceIdx : SV_InstanceID
#endif
)
#else
VsOutput VS_Main(
  float4 Position : POSITION
, float3 Normal : NORMAL
, float2 Uv : TEXCOORD0
#ifdef FERTILIZER
, float FertilizerVertexTime : TIME_STREAM
#endif
, uint VertexIdx : SV_VertexID
#ifdef INSTANCED_TRANSFORM
, uint InstanceIdx : SV_InstanceID
#endif
)
#endif
{
	VsOutput Out = (VsOutput)0;
	
#ifdef DEFORMERS_ENABLED
	float3 undeformedPosition;
	float3 Position = EvaluatePosition(undeformedPosition, VertexIdx);
	float3 normal = DeformedNormalBuffer[VertexIdx + VertexBaseIndex];
	float2 Uv = DeformedUVBuffer[VertexIdx + VertexBaseIndex];
	float FertilizerVertexTime = 0.0f;
#else
	float3 normal = Normal.zyx * 2.0f - 1.0f;
	float3 undeformedPosition = Position.xyz;
#endif	

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

	float4 textureColour = MaterialColourTexture.Sample(AnisoWrapSampler, In.Uv);
	return In.Colour * textureColour * (MaterialDiffuse + MaterialLuminosity);
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
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
#else
VsOutput VS_GBuffer(
float4 Position : POSITION, 
float3 Normal : NORMAL, 
float2 Uv : TEXCOORD0, 
float4 Colour : COLOR0,
#ifdef FERTILIZER
float FertilizerVertexTime : TIME_STREAM,
#endif
uint VertexIdx : SV_VertexID
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
#endif
{
	VsOutput Out = (VsOutput)0;
	
	
#ifdef DEFORMERS_ENABLED
	float3 undeformedPosition;
	float3 Position = EvaluatePosition(undeformedPosition, VertexIdx);
	float3 normal = DeformedNormalBuffer[VertexIdx + VertexBaseIndex];
	float2 Uv = DeformedUVBuffer[VertexIdx + VertexBaseIndex];
	float4 Colour = UnpackRGBA8(DeformedColourBuffer[VertexIdx + VertexBaseIndex]);
	float FertilizerVertexTime = 0.0f;
#else
	float3 normal = Normal.zyx * 2.0f - 1.0f;
	float3 undeformedPosition = Position.xyz;
#endif		
	
	float3 worldPosition = mul( float4(Position.xyz,1), World ).xyz;
	float3 worldNormal = normalize(mul(float4(normal,0), World).xyz);
#ifdef INSTANCED_TRANSFORM		
	worldPosition.xyz = mul(float4(worldPosition.xyz, 1.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz;
	worldNormal.xyz = normalize(mul(float4(worldNormal.xyz, 0.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz);
#endif
	
#ifdef FERTILIZER
	Out.FertilizerVertexTime = FertilizerVertexTime;
#endif	
	
	Out.WorldPosition = worldPosition;
#ifdef TESSELLATION_ENABLED	
	Out.Position = mul( float4(Position.xyz,1), World );
	Out.TessFactor = MaxTessellationFactor;
#else	
	Out.Position = mul( float4(worldPosition.xyz,1), ViewProjection );
#endif
	Out.Normal = normalize(mul(float4(worldNormal,0), View).xyz);
	Out.Uv = Uv;
	Out.Colour = float4(Colour.xyz,1.0f) * MaterialColour;
	Out.LocalPosition = undeformedPosition;

	return Out;
}



#ifdef GEOMETRY_DISPLACEMENT

[maxvertexcount(15)] void GS_RenderGeometryDisplacement( triangle VsOutput input[3], inout TriangleStream<VsOutput> OutputStream )
{	
	float3 pos0 = input[0].WorldPosition.xyz;
	float3 pos1 = input[1].WorldPosition.xyz;
	float3 pos2 = input[2].WorldPosition.xyz;

	float3 v0 = pos1 - pos0;
	float3 v1 = pos2 - pos0;
	float3 v = cross(v0,v1);
	float area = 0.5f * (abs(v.x) + abs(v.y) + abs(v.z));
	
	float3 centrePos = (pos0+pos1+pos2) * (1.0f/3.0f);
	float noiseAmount = max(Noise3f(centrePos*50.0f)-0.2f,0);
	
	float extrudeAmount = area * noiseAmount;
	
	float bevelAmt = 0.1f;
			
	if(extrudeAmount > 0.0f)
	{
		float3 nrm = normalize(cross(normalize(v0),normalize(v1)));
		
		extrudeAmount = 0.2f * noiseAmount;
		
		float3 extruded0 = lerp(pos0,centrePos,bevelAmt) + nrm * extrudeAmount;
		float3 extruded1 = lerp(pos1,centrePos,bevelAmt) + nrm * extrudeAmount;
		float3 extruded2 = lerp(pos2,centrePos,bevelAmt) + nrm * extrudeAmount;
		
		float4 e0vp = mul(float4(extruded0,1.0f), ViewProjection);
		float4 e1vp = mul(float4(extruded1,1.0f), ViewProjection);
		float4 e2vp = mul(float4(extruded2,1.0f), ViewProjection);
		
		VsOutput Out = input[0];
		
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
	}
	else
	{
		OutputStream.Append( input[0] );
		OutputStream.Append( input[1] );
		OutputStream.Append( input[2] );
	}
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
	float noiseAmount = max(Noise3f(centrePos*50.0f)-0.2f,0);
	
	float extrudeAmount = area * noiseAmount;
	
	float bevelAmt = 0.1f;
			
	if(extrudeAmount > 0.0f)
	{
		float3 nrm = normalize(cross(normalize(v0),normalize(v1)));
		
		extrudeAmount = 0.2f * noiseAmount;
		
		float3 extruded0 = lerp(pos0,centrePos,bevelAmt) + nrm * extrudeAmount;
		float3 extruded1 = lerp(pos1,centrePos,bevelAmt) + nrm * extrudeAmount;
		float3 extruded2 = lerp(pos2,centrePos,bevelAmt) + nrm * extrudeAmount;
		
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
	}
	else
	{
		OutputStream.Append( input[0] );
		OutputStream.Append( input[1] );
		OutputStream.Append( input[2] );
	}
}



#endif // GEOMETRY_DISPLACEMENT



float4 PS_RenderPixelCounts( PsInput In, bool IsFrontFace : SV_IsFrontFace ) : SV_TARGET0
{
#ifdef FERTILIZER
	clip(GetFertilizerClipValue(In.FertilizerVertexTime));
#endif

	
#ifdef USE_GENERATED_SHADER
	if(IsFrontFace)
	{
		float d = EvaluateDistanceFieldLinear(In.WorldPosition);	
		clip(-d);
	}
#endif

	return 1.0f;
}



float4 PS_RenderPixelSamples( PsInput In, bool IsFrontFace : SV_IsFrontFace ) : SV_TARGET0
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
		float d = EvaluateDistanceFieldLinear(In.WorldPosition);	
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


float4 PS_RenderNormalDepthPixelSamples( PsInput In, bool IsFrontFace : SV_IsFrontFace ) : SV_TARGET0
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
	if(!IsFrontFace)
		faceNormal *= -1.0f;

	normal = normalize(lerp(faceNormal, normal, saturate(NormalSmoothing)));
	

	if(isVisible)
	{	
		float viewDepth = mul(float4(In.WorldPosition,1.0f),View).z;
		//float3 viewNormal = normalize(mul(float4(normal,0.0f),View).xyz);
					
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
	float d = EvaluateDistanceFieldLinear(In.WorldPosition);
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
		
	float4 textureColour = MaterialColourTexture.SampleBias(AnisoWrapSampler, In.Uv, -1.0f);
	float3 proceduralColour = EvaluateProceduralTextureGrid(In.LocalPosition);
	textureColour.xyz = lerp(textureColour.xyz, proceduralColour, ProceduralTextureWeight);
	
	float3 outColour = In.Colour.xyz * textureColour.xyz;
	
	GBufferOutput Out;
	Out.ColourObjectId = float4(outColour,ObjectIndex);
	Out.NormalMaterialId = float4(normalize(normal.xyz) * 0.5f + 0.5f, MaterialIndex);
	
	return Out;
}



#ifdef DEFORMERS_ENABLED
VsShadowOutput VS_Shadow(
uint VertexIdx : SV_VertexID
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
#else
VsShadowOutput VS_Shadow(
float4 Position : POSITION
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
#endif
{
	VsShadowOutput Out = (VsShadowOutput)0;
	
#ifdef DEFORMERS_ENABLED
	float3 Position;
	float3 undeformedPosition;
	Position.xyz = EvaluatePosition(undeformedPosition, VertexIdx);
#endif

	float3 worldPosition = mul( float4(Position.xyz,1), World ).xyz;
#ifdef INSTANCED_TRANSFORM		
	worldPosition.xyz = mul(float4(worldPosition.xyz, 1.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz;
#endif

	Out.Position = mul( float4(worldPosition.xyz,1), ViewProjection );
	return Out;
}
float4 PS_Shadow(VsShadowOutput In) : SV_TARGET
{ 
#ifdef USE_GENERATED_SHADER
//	float d = EvaluateDistanceFieldLinear(In.WorldPosition);
//	clip(d);
#endif
	return 1.0f;
}




#ifdef DEFORMERS_ENABLED
VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT VS_CaptureTriangleOctreeGeometry(
uint VertexIdx : SV_VertexID
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
#else
VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT VS_CaptureTriangleOctreeGeometry(
float4 Position : POSITION, 
float3 Normal : NORMAL, 
float2 Uv : TEXCOORD0, 
uint VertexIdx : SV_VertexID
#ifdef INSTANCED_TRANSFORM		
, uint InstanceIdx : SV_InstanceID
#endif
)
#endif
{
	VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT Out = (VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT)0;
		
#ifdef DEFORMERS_ENABLED
	float3 undeformedPosition;
	float3 Position = EvaluatePosition(undeformedPosition, VertexIdx);
	float3 normal = DeformedNormalBuffer[VertexIdx + VertexBaseIndex];
	float2 Uv = DeformedUVBuffer[VertexIdx + VertexBaseIndex];
#else
	float3 normal = Normal.zyx * 2.0f - 1.0f;
	float3 undeformedPosition = Position.xyz;
#endif		
	
	float3 worldPosition = mul( float4(Position.xyz,1), World ).xyz;
	float3 worldNormal = normalize(mul( float4(normal.xyz,0), World ).xyz);
#ifdef INSTANCED_TRANSFORM		
	worldPosition.xyz = mul(float4(worldPosition.xyz, 1.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz;
	worldNormal.xyz = normalize(mul(float4(worldNormal.xyz, 0.0f), CloneInstanceTransformBuffer[InstanceIdx]).xyz);
#endif
		
	Out.Position = mul( float4(worldPosition.xyz,1), ViewProjection );
	Out.Uv = Uv;
	Out.Colour = 1.0f;
	Out.Normal = normalize(mul( float4(worldNormal.xyz,0), ViewProjection ).xyz);
	
	return Out;	
}

[maxvertexcount(1)] void GS_CaptureTriangleOctreeGeometry2( triangle VS_TRIANGLE_OCTREE_CAPTURE_OUTPUT input[3], inout PointStream<PS_TRIANGLE_OCTREE_CAPTURE_INPUT> OutputSliceStream, uint TriangleIndex : SV_PRIMITIVEID )
{
	PS_TRIANGLE_OCTREE_CAPTURE_INPUT Out;
	Out.V0 = input[0].Position.xyz;
	Out.V1 = input[1].Position.xyz;
	Out.V2 = input[2].Position.xyz;
	Out.Uvs01 = float4(input[0].Uv,input[1].Uv);
	Out.Uv2 = input[2].Uv;
	
	float3 faceNormal = normalize(cross(normalize(input[1].Position.xyz-input[0].Position.xyz),normalize(input[2].Position.xyz-input[0].Position.xyz)));
	input[0].Normal.xyz = normalize(lerp(faceNormal, input[0].Normal.xyz, NormalSmoothing));
	input[1].Normal.xyz = normalize(lerp(faceNormal, input[1].Normal.xyz, NormalSmoothing));
	input[2].Normal.xyz = normalize(lerp(faceNormal, input[2].Normal.xyz, NormalSmoothing));
	
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




[numthreads(64, 1, 1)]
void CS_CaptureTriangleOctreeGeometry(uint3 GroupId : SV_GroupID, 
						uint3 DispatchThreadId : SV_DispatchThreadID, 
						uint3 GroupThreadId : SV_GroupThreadID,
						uint GroupIndex : SV_GroupIndex)
{
	uint instancedTriangleIndex = DispatchThreadId.x;
	
	if(instancedTriangleIndex < NumTriangles)
	{
	
		TriangleData tri = (TriangleData)0;
		
#ifdef INSTANCED_TRANSFORM		
		uint triangleIndex = instancedTriangleIndex / InstanceCount;	
#else
		uint triangleIndex = instancedTriangleIndex;
#endif
		uint3 indices = uint3(IndexBuffer[triangleIndex*3 + 0], IndexBuffer[triangleIndex*3 + 1], IndexBuffer[triangleIndex*3 + 2]);
		
		float3 undeformedPos0,undeformedPos1,undeformedPos2;
		float3 pos0 = EvaluatePosition(undeformedPos0, indices.x);
		float3 pos1 = EvaluatePosition(undeformedPos1, indices.y);
		float3 pos2 = EvaluatePosition(undeformedPos2, indices.z);
		
		uint vertexBaseIdx = 0;
#ifdef DEFORMERS_ENABLED
		vertexBaseIdx = VertexBaseIndex;
		float3 normal0 = DeformedNormalBuffer[indices.x + vertexBaseIdx];
		float3 normal1 = DeformedNormalBuffer[indices.y + vertexBaseIdx];
		float3 normal2 = DeformedNormalBuffer[indices.z + vertexBaseIdx];
#else
		float3 normal0 = normalize(UnpackRGBA8(DeformedNormalBuffer[indices.x]).zyx * 2.0f - 1.0f);
		float3 normal1 = normalize(UnpackRGBA8(DeformedNormalBuffer[indices.y]).zyx * 2.0f - 1.0f);
		float3 normal2 = normalize(UnpackRGBA8(DeformedNormalBuffer[indices.z]).zyx * 2.0f - 1.0f);
#endif
		
		tri.m_uv0 = DeformedUVBuffer[indices.x + vertexBaseIdx];
		tri.m_uv1 = DeformedUVBuffer[indices.y + vertexBaseIdx];
		tri.m_uv2 = DeformedUVBuffer[indices.z + vertexBaseIdx];
		tri.m_materialIndex = MaterialIndex;
		tri.m_objectIndex = ObjectIndex;
		tri.m_colour0 = DeformedColourBuffer[indices.x + vertexBaseIdx];
		tri.m_colour1 = DeformedColourBuffer[indices.y + vertexBaseIdx];
		tri.m_colour2 = DeformedColourBuffer[indices.z + vertexBaseIdx];
		
#ifdef INSTANCED_TRANSFORM	
		uint instanceIndex = instancedTriangleIndex % InstanceCount;
		
		pos0 = mul(float4(pos0.xyz,1), World ).xyz;
		pos1 = mul(float4(pos1.xyz,1), World ).xyz;
		pos2 = mul(float4(pos2.xyz,1), World ).xyz;
		normal0 = normalize(mul(float4(normal0.xyz,0), World ).xyz);
		normal1 = normalize(mul(float4(normal1.xyz,0), World ).xyz);
		normal2 = normalize(mul(float4(normal2.xyz,0), World ).xyz);
		
		pos0 = mul(float4(pos0.xyz,1), CloneInstanceTransformBuffer[instanceIndex] ).xyz;
		pos1 = mul(float4(pos1.xyz,1), CloneInstanceTransformBuffer[instanceIndex] ).xyz;
		pos2 = mul(float4(pos2.xyz,1), CloneInstanceTransformBuffer[instanceIndex] ).xyz;
		normal0 = normalize(mul(float4(normal0.xyz,0), CloneInstanceTransformBuffer[instanceIndex] ).xyz);
		normal1 = normalize(mul(float4(normal1.xyz,0), CloneInstanceTransformBuffer[instanceIndex] ).xyz);
		normal2 = normalize(mul(float4(normal2.xyz,0), CloneInstanceTransformBuffer[instanceIndex] ).xyz);
		
		pos0 = mul(float4(pos0.xyz,1), ViewProjection ).xyz;
		pos1 = mul(float4(pos1.xyz,1), ViewProjection ).xyz;
		pos2 = mul(float4(pos2.xyz,1), ViewProjection ).xyz;
		normal0 = normalize(mul(float4(normal0.xyz,0), ViewProjection ).xyz);
		normal1 = normalize(mul(float4(normal1.xyz,0), ViewProjection ).xyz);
		normal2 = normalize(mul(float4(normal2.xyz,0), ViewProjection ).xyz);
				
#else
		pos0 = mul(float4(pos0.xyz,1), WorldViewProjection ).xyz;
		pos1 = mul(float4(pos1.xyz,1), WorldViewProjection ).xyz;
		pos2 = mul(float4(pos2.xyz,1), WorldViewProjection ).xyz;
		normal0 = normalize(mul(float4(normal0.xyz,0), WorldViewProjection ).xyz);
		normal1 = normalize(mul(float4(normal1.xyz,0), WorldViewProjection ).xyz);
		normal2 = normalize(mul(float4(normal2.xyz,0), WorldViewProjection ).xyz);
#endif	
			
		
		float3 faceNormal = normalize(cross(normalize(pos1-pos0),normalize(pos2-pos0)));
		normal0 = normalize(lerp(faceNormal, normal0, NormalSmoothing));
		normal1 = normalize(lerp(faceNormal, normal1, NormalSmoothing));
		normal2 = normalize(lerp(faceNormal, normal2, NormalSmoothing));
		normal0 = normal0 * 0.5f + 0.5f;
		normal1 = normal1 * 0.5f + 0.5f;
		normal2 = normal2 * 0.5f + 0.5f;
		
		tri.m_v0 = pos0;
		tri.m_v1 = pos1;
		tri.m_v2 = pos2;
		uint3 n0 = uint3(normal0 * 255.0f);
		uint3 n1 = uint3(normal1 * 255.0f);
		uint3 n2 = uint3(normal2 * 255.0f);
		tri.m_normal0 = n0.x | (n0.y << 8) | (n0.z << 16);
		tri.m_normal1 = n1.x | (n1.y << 8) | (n1.z << 16);
		tri.m_normal2 = n2.x | (n2.y << 8) | (n2.z << 16);

		float3 areaVec = cross(tri.m_v1-tri.m_v0,tri.m_v2-tri.m_v0);
	//	float3 vec0 = tri.m_v1-tri.m_v0;
	//	float3 vec1 = tri.m_v2-tri.m_v0;
	//	float3 vec2 = tri.m_v2-tri.m_v1;
	//	float s0 = length(vec0);
	//	float s1 = length(vec1);
	//	float s2 = length(vec2);
	//	float p = (s0+s1+s2)*0.5f;
	//	float area = sqrt(p * (p-s0) * (p-s1) * (p-s2));

		//float area = length(areaVec);
		//if(area > 0.0000001f)
		if(any(areaVec > 0.000000001f))
		{
			RWSceneTriangleBuffer.Append(tri);
		}
		
	}
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
  DepthEnable = TRUE;
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
#ifdef GEOMETRY_DISPLACEMENT	
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
#ifdef GEOMETRY_DISPLACEMENT	
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
#ifdef GEOMETRY_DISPLACEMENT	
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
#ifdef GEOMETRY_DISPLACEMENT	
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
#ifdef GEOMETRY_DISPLACEMENT	
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
#ifdef GEOMETRY_DISPLACEMENT	
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
#ifdef GEOMETRY_DISPLACEMENT	
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
#ifdef GEOMETRY_DISPLACEMENT	
		SetGeometryShader( CompileShader( gs_5_0, GS_RenderGeometryDisplacement() ) );
#endif		
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
        SetGeometryShader( CompileShader( gs_5_0, GS_CaptureTriangleOctreeGeometry2() ) );
        SetPixelShader( CompileShader( ps_5_0, PS_CaptureTriangleOctreeGeometry() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( NoDepthState, 0);
		SetRasterizerState( DefaultRasterState );
    }    
}


technique11 CopyTriangleOctreeGeometry
{
    pass p0
    {   
        SetComputeShader( CompileShader( cs_5_0, CS_CaptureTriangleOctreeGeometry() ) );
    }    
}
