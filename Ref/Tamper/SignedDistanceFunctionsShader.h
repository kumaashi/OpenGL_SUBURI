
#ifndef SIGNED_DISTANCE_FUNCTIONS_SHADER_H
#define SIGNED_DISTANCE_FUNCTIONS_SHADER_H

float BoxDistance( float3 p, float3 b )
{
  float3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0f) + length(max(d,0.0f));
}

float SphereDistance( float3 p, float radius )
{
  return length(p)-radius;
}

float TorusDistance( float3 p, float2 t )
{
  float2 q = float2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float CylinderDistance( float3 p, float3 c )
{
  return length(p.xz-c.xy)-c.z;
}

float ConeDistance( float3 p, float2 c )
{
    // c must be normalized
    float q = length(p.xy);
    return dot(c,float2(q,p.z));
}

float PlaneDistance( float3 p, float4 n )
{
  // n must be normalized
  return dot(p,n.xyz) + n.w;
}

float HexPrismDistance( float3 p, float2 h )
{
    float3 q = abs(p);
    return max(q.z-h.y,max(q.x+q.y*0.57735,q.y*1.1547)-h.x);
}

float TriPrismDistance( float3 p, float2 n )
{
    float3 q = abs(p);
    return max(q.z-n.y,max(q.x*0.866025+p.y*0.5,-p.y)-n.x*0.5);
}

#endif
