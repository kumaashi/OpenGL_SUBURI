#ifndef SHADER_RANDOM_H
#define SHADER_RANDOM_H


int IRand(int n)
{
	n = (n << 13) ^ n;
	return (n * (n*n*15731+789221) + 1376312589) & 0x7fffffff;
}

float FRand( int n )
{
  return float(IRand(n));
}

float NormalisedRand( int n )
{
	return float(IRand(n)) * (1.0f/2147483647.0f);
}

// iq's integerless noise
float HashNoise( float n )
{
    return frac(sin(n)*43758.5453);
}

float Noise3f(in float3 p)
{
  int3 ip = int3(floor(p));
  float3 u = frac(p);
  u = u*u*(3.0-2.0*u);

  int n = ip.x + ip.y*57 + ip.z*113;

  float res = lerp(lerp(lerp(FRand(n+(0+57*0+113*0)),
                          FRand(n+(1+57*0+113*0)),u.x),
                      lerp(FRand(n+(0+57*1+113*0)),
                          FRand(n+(1+57*1+113*0)),u.x),u.y),
                 lerp(lerp(FRand(n+(0+57*0+113*1)),
                          FRand(n+(1+57*0+113*1)),u.x),
                      lerp(FRand(n+(0+57*1+113*1)),
                          FRand(n+(1+57*1+113*1)),u.x),u.y),u.z);

  return 1.0f - res*(1.0f/1073741824.0f);
}

float HashNoise3f( in float3 x )
{
    float3 p = floor(x);
    float3 f = frac(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0 + 113.0*p.z;

    float res = lerp(lerp(lerp( HashNoise(n+  0.0), HashNoise(n+  1.0),f.x),
                        lerp( HashNoise(n+ 57.0), HashNoise(n+ 58.0),f.x),f.y),
                    lerp(lerp( HashNoise(n+113.0), HashNoise(n+114.0),f.x),
                        lerp( HashNoise(n+170.0), HashNoise(n+171.0),f.x),f.y),f.z);
    return res;
}



float Fbm( float3 p, uint numOctaves )
{
	float f = 0;
	float s = 0.5f;
	[unroll] for(uint i = 0; i < numOctaves; ++i)
	{
		f += Noise3f( p )*s;
		p *= 2.0f;
		s *= 0.5f;
	}
    return f;
}

float FbmHashNoise( float3 p, uint numOctaves )
{
	float f = 0;
	float s = 0.5f;
	[unroll] for(uint i = 0; i < numOctaves; ++i)
	{
		f += HashNoise3f( p )*s;
		p *= 2.0f;
		s *= 0.5f;
	}
    return f;
}
float FbmRotated( float3 p )
{
	// matrix to rotate the noise octaves
	float3x3 m = float3x3( 0.00,  0.80,  0.60,
						  -0.80,  0.36, -0.48,
						  -0.60, -0.48,  0.64 );

    float f;
    f = 0.5000*HashNoise3f( p ); 
    p = mul(p,m)*2.02;
    f += 0.2500*HashNoise3f( p ); 
    p = mul(p,m)*2.03;
    f += 0.1250*HashNoise3f( p ); 
    p = mul(p,m)*2.01;
    f += 0.0625*HashNoise3f( p );
    
    return f;
}

/*
#define EvaluateFbm(pos, numOctaves, NoiseFunction) \
{ \
	float f = 0; \
	float s = 0.5f; \
	float3 p = pos; \
	for(int ni = 0; ni < numOctaves; ++ni) {\
		f += NoiseFunction(p) * s; s*=0.5f;\
	}\
*/

float3 GenerateRandomisedDirection(float3 dir, float spread, int seed)
{	
	float r1 = (NormalisedRand(seed + 17) * 6.21f);
	float r2 = NormalisedRand(seed+191) * spread;
	float r2s = sqrt(r2);
	float3 w = dir;
	float3 u = abs(w.x) > 0.3f ? float3(0.0f,1.0f,0.0f) : float3(1.0f,0.0f,0.0f);
	u = normalize(cross(u, w));
	float3 v = normalize(cross(w,u));
	float3 d = normalize( u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2) ); 
	return d;
}


#endif
