#ifndef FAST_PERLIN_SHADER_H
#define FAST_PERLIN_SHADER_H

Texture3D <float4> Noise3DTexture : NOISE3DTEXTURE;

sampler Noise3DSampler
{
	Filter = Min_Mag_Linear_Mip_Point;
    AddressU = Wrap;
    AddressV = Wrap;
	AddressW = Wrap;
};


float noiseFast(float3 p)
{
	p *= 0.125f; // because the volume texture is 8x8x8 noise, divide the position by 8 to keep this noise in parity with the true Perlin noise generator.
	float2 hi = Noise3DTexture.SampleLevel(Noise3DSampler, p, 0).xy * 2.0f - 1.0f; // High frequency noise
	float   lo = Noise3DTexture.SampleLevel(Noise3DSampler, p * (1.0f/9.0f), 0).x * 2.0f - 1.0f; // Low frequency noise

	float  angle = lo*2.0f*3.141f;
	float result = hi.x * cos(angle) + hi.y * sin(angle); // Use the low frequency as a quaternion rotation of the high-frequency's real and imaginary parts.
	return result; // done!
}

// returns 3 components of noise
float3 noiseFast3(float3 p)
{
	p *= 0.125f; // because the volume texture is 8x8x8 noise, divide the position by 8 to keep this noise in parity with the true Perlin noise generator.
	float4 hi = Noise3DTexture.SampleLevel(Noise3DSampler, p, 0) * 2.0f - 1.0f; // High frequency noise
	float3  lo = Noise3DTexture.SampleLevel(Noise3DSampler, p * (1.0f/9.0f), 0).xyz * 2.0f - 1.0f; // Low frequency noise
	float3  angle = lo*2.0f*3.141f;

	float3 result;
	result.x = hi.x * cos(angle.x) + hi.y * sin(angle.x);
	result.y = hi.y * cos(angle.y) + hi.z * sin(angle.y);
	result.z = hi.z * cos(angle.z) + hi.w * sin(angle.z);

	return result; // done!
}


float fbm(float3 p, int octaves, float lacunarity = 2.0, float gain = 0.5)
{
	float freq = 1.0, amp = 0.5;
	float sum = 0;	
	for(int i=0; i<octaves; i++) {
		sum += noiseFast(p*freq)*amp;
		freq *= lacunarity;
		amp *= gain;
	}
	return sum;
}
float3 fbm3(float3 p, int octaves, float lacunarity = 2.0, float gain = 0.5)
{
	float freq = 1.0, amp = 0.5;
	float3 sum = 0;	
	for(int i=0; i<octaves; i++) {
		sum += noiseFast3(p*freq)*amp;
		freq *= lacunarity;
		amp *= gain;
	}
	return sum;
}

#endif