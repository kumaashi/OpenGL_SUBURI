Texture2D <float4> TextTexture : TEXTMAP;

sampler LinearSampler
{
	Filter = Min_Mag_Linear_Mip_Point;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VsOutput
{
    float4 Position		: SV_POSITION;  
	float2 Uv			: TEXCOORD0;
};

VsOutput VS_Main(float4 Position : POSITION)
{
	VsOutput Out = (VsOutput)0;
	
	Out.Position = float4(Position.xyz,1);
	Out.Uv = float2(Position.xy * 0.5f + 0.5f);
	
	return Out;
}
float4 PS_Main( VsOutput In ) : SV_TARGET
{ 
	float4 texVal = TextTexture.Sample(LinearSampler, In.Uv);
	return float4(0.8,0.8,0.8,texVal.x);
}

BlendState NoBlend {
  AlphaToCoverageEnable = FALSE;
  BlendEnable[0] = TRUE;
  SrcBlend[0] = Src_Alpha;
  DestBlend[0] = Inv_Src_Alpha;
  SrcBlendAlpha[0] = Src_Alpha;
  DestBlendAlpha[0] = Inv_Src_Alpha;
};
   
DepthStencilState DepthState {
  DepthEnable = FALSE;
};

RasterizerState DefaultRasterState 
{
	CullMode = None;
	FillMode = Solid;
	DepthBias = 0;
	ScissorEnable = false;
};

technique11 RenderText
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, VS_Main() ) );
		SetPixelShader( CompileShader( ps_4_0, PS_Main() ) );
		
		SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
		SetDepthStencilState( DepthState, 0);
		SetRasterizerState( DefaultRasterState );
	}
}
