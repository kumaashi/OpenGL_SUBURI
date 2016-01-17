
float EvaluateDistanceFieldCubic(float3 pos, inout float4 closestObjectColour)
{
	pos = mul(float4(pos.xyz* 0.5 + 0.5,1.0f),GridToWorldTransform).xyz;
	float sdfValue = 0;
	closestObjectColour = 1.0f;
	closestObjectColour = float4(0.0f,0.0f,0.0f,0.0000001f);
	float closestObjectDistance = 10000.0f;
float EvaluateDistanceFieldLinear(float3 pos, inout float4 closestObjectColour)
	closestObjectColour /= closestObjectColour.w;
	return sdfValue * DistanceSign;
}
#define USE_GENERATED_SHADER
#define USE_BOOLEANS
int3 GridPixelSize : GRIDPIXELSIZE;
float DistanceSign : DISTANCESIGN;
float4x4 GridToWorldTransform : GRIDTOWORLDTRANSFORM;
sampler LinearFilterSampler 
 { 
    Filter = Min_Mag_Linear_Mip_Point;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
    };
float EvaluateDistanceFieldLinear(float3 pos)
		float4 col;
		return EvaluateDistanceFieldLinear(pos, col);
