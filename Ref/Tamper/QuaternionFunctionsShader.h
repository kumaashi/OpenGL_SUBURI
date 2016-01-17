
#ifndef QUATERNION_FUNCTIONS_SHADER_H
#define QUATERNION_FUNCTIONS_SHADER_H


// --------- quaternion representation -----------------------------------------
//
// Each quaternion can be specified by four scalars q = A + Bi + Cj + Dk, so are
// stored as a float4.  I've tried a struct containing a separate scalar and
// 3-vector to avoid a lot of swizzling, but the float4 representation ends up
// using fewer instructions.  A matrix representation is also possible.
//

float4 QuatBuild( float3 xyzComponent, float wComponent)
{
	return float4(wComponent, xyzComponent.x, xyzComponent.y, xyzComponent.z);
}

float4 QuatMul( float4 q1, float4 q2 )
{
   float4 r;

   r.x   = q1.x*q2.x - dot( q1.yzw, q2.yzw );
   r.yzw = q1.x*q2.yzw + q2.x*q1.yzw + cross( q1.yzw, q2.yzw );

   return r;
}
float4 QuatSq( float4 q )
{
   float4 r;

   r.x   = q.x*q.x - dot( q.yzw, q.yzw );
   r.yzw = 2*q.x*q.yzw;

   return r;
}
float4 QuatConjugate(float4 q1) 
{
	return float4(q1.x, -q1.yzw);
}

float4 QuatNormalise(float4 q) 
{
    float n = 1.0f / sqrt(dot(q,q));
    return q * n;
}

float4 QuatFromAxisAngle(float3 axis, float angle)
{
	float halfangle = 0.5f*angle;	
	float4 q;
	q.x = cos(halfangle);
	q.yzw = axis * sin(halfangle);
	
	return q;
}


float3 RotatePointByQuaternion(float3 pos, float4 rot)
{
	float4 posQ = float4(1.0f,pos.x,pos.y,pos.z);
	float4 rotConj = QuatConjugate(rot);

	float4 rslt = QuatMul(rotConj,posQ);
	rslt = QuatMul(rslt,rot);
	
	return rslt.yzw;
}

float3 RotateDirectionByQuaternion(float3 pos, float4 rot)
{
	float4 posQ = float4(0.0f,pos.x,pos.y,pos.z);
	float4 rotConj = QuatConjugate(rot);

	float4 rslt = QuatMul(rotConj,posQ);
	rslt = QuatMul(rslt,rot);
	
	return rslt.yzw;
}


float4x4 QuatToMatrix(float4 q)
{
	float xx = q.y * q.y;
	float xy = q.y * q.z;
	float xz = q.y * q.w;
	float xw = q.y * q.x;

	float yy = q.z * q.z;
	float yz = q.z * q.w;
	float yw = q.z * q.x;

	float zz = q.w * q.w;
	float zw = q.w * q.x;

	float4x4 mat;
	mat[0] = float4( 1.0f - 2.0f * ( yy + zz ), 2.0f * ( xy - zw ), 2.0f * ( xz + yw ), 0.0f );
	mat[1] = float4( 2.0f * ( xy + zw ), 1.0f - 2.0f * ( xx + zz ), 2.0f * ( yz - xw ), 0.0f );
	mat[2] = float4( 2.0f * ( xz - yw ), 2.0f * ( yz + xw ), 1.0f - 2.0f * ( xx + yy ), 0.0f );
	mat[3] = float4( 0.0f, 0.0f, 0.0f, 1.0f);

	return mat;
}



#endif
