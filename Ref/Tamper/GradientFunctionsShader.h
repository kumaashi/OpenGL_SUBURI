
#ifndef GRADIENT_FUNCTIONS_SHADER_H
#define GRADIENT_FUNCTIONS_SHADER_H


float4x4 GradientTransform : GRADIENTTRANSFORM;
float GradientInnerRange : GRADIENTINNERRANGE;
float GradientOuterRange : GRADIENTOUTERRANGE;
float GradientFalloffPower : GRADIENTFALLOFFPOWER;
float4 GradientColour0 : GRADIENTCOLOUR0;
float4 GradientColour1 : GRADIENTCOLOUR1;
uint GradientIsLinear : GRADIENTISLINEAR;


float4 EvaluateGradient2D(float2 screenPos)
{
	float3 gridPos = mul(float4(screenPos,0,1),GradientTransform);
	
	float linearDist = abs(gridPos.y);
	float radialDist = length(gridPos);
	float dist = GradientIsLinear ? linearDist : radialDist;
	float gradientValue = saturate((dist - GradientInnerRange) / (GradientOuterRange - GradientInnerRange));
	gradientValue = pow(gradientValue, GradientFalloffPower);
	
	float4 gradientColour = lerp(GradientColour0, GradientColour1, gradientValue);
	
	return gradientColour;
}


#endif
