#ifndef _VECMAT_H_
#define _VECMAT_H_

#include "warning.h"

//--------------------------------------------------------------------------------------
// math
//--------------------------------------------------------------------------------------
#ifndef PI
#define  PI  3.141592653
#endif

#define d2r(x)  (((x) * PI * 2) / 180.0)

inline float clamp(float x, float a, float b) {
	return (std::min)((std::max)(x, a), b);
}

inline float lerp(float x, float y, float a) {
	return x * (1.0 - a) + y * a;
}

inline float mix(float x, float y, float a) {
	return lerp(x, y, a);
}

inline float lerpc(float a, float b, float x) { 
	float ft = x * PI;
	float  f = (1.0 - cos(ft)) * 0.5;
	return a * (1.0 - f) + b * f;
}

inline float smoothstep(float e0, float e1, float x) {
	float t = clamp((x - e0) / (e1 - e0), 0.0, 1.0);
	return t * t * (3.0 - 2.0 * t);
}


//--------------------------------------------------------------------------------------
// vec
//--------------------------------------------------------------------------------------
#define _VEC3_DEFOP_(o)  vec operator o (const vec &a) { return vec(x o a.x, y o a.y, z o a.z, w o a.w); }
__declspec(align(256)) struct vec {
	union {
		struct { float x, y, z, w; };
		float data[4];
	};

	void print() {
		printf("%08X: %5.4f %5.4f %5.4f %5.4f\n", (unsigned long )this, x, y, z, w);
	}

	vec() {
		x = y = z = 0;
		w = 1;
	}

	vec(float a) {
		x = y = z = a;
		w = 1;
	}

	vec(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
		w = 1;
	}

	vec(float a, float b, float c, float d) {
		x = a;
		y = b;
		z = c;
		w = d;
	}

	float dot(vec &a) {
		return x * a.x + y * a.y + z * a.z;
	}

	float dot() {
		return dot(*this);
	}

	float length()	{
		return length(*this);
	}

	float length(vec &a)	{
		return sqrt(a.dot(a));
	}

	vec cross(vec &a, vec &b) {
		return vec(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	}

	vec cross(vec &b) {
		return cross(*this, b);
	}

	vec normalize() {
		float l = 1.0 / length();
		x *= l;
		y *= l;
		z *= l;
		return *this;
	}

	vec smoothstep(vec &a, vec &b, float k) {
		float (*fn)(float, float, float) = ::smoothstep;
		/*
		x = fn(a.x, b.x, k);
		y = fn(a.y, b.y, k);
		z = fn(a.z, b.z, k);
		*/
		float bk  = fn(1,0,k);
		float ibk = 1 - bk;
		x = (bk * a.x) + (ibk * b.x);
		y = (bk * a.y) + (ibk * b.y);
		z = (bk * a.z) + (ibk * b.z);
		return *this;
	}

	vec lerp(vec &a, vec &b, float k) {
		float (*fn)(float, float, float) = ::lerp;
		x = fn(a.x, b.x, k);
		y = fn(a.y, b.y, k);
		z = fn(a.z, b.z, k);
		return *this;
	}

	vec lerpc(vec &a, vec &b, float k) {
		float (*fn)(float, float, float) = ::lerpc;
		x = fn(a.x, b.x, k);
		y = fn(a.y, b.y, k);
		z = fn(a.z, b.z, k);
		return *this;
	}

	
	_VEC3_DEFOP_( +)	 _VEC3_DEFOP_( -)	 _VEC3_DEFOP_( *)	 _VEC3_DEFOP_( /) 
	_VEC3_DEFOP_(+=)	 _VEC3_DEFOP_(-=)	 _VEC3_DEFOP_(*=)	 _VEC3_DEFOP_(/=) 
	
};


//--------------------------------------------------------------------------------------
// Matrix
//--------------------------------------------------------------------------------------
struct Matrix {
	float data[16];
	
	void Print() {
		printf("======================================================");
		for(int i = 0 ; i < 16; i++) {
			if( (i % 4) == 0) printf("\n");
			printf("%5.3f, ", data[i]);
		}
		printf("\n======================================================\n");
	}

	float *Ident() {
		float *d = data;
		memset(d, 0, sizeof(float) * 16);
		d[0]	= 1.0;
		d[5]	= 1.0;
		d[10] = 1.0;
		d[15] = 1.0;
		return d;
	}
	


	float *Scale(float x, float y, float z) {
		float *d = data;
		d[0]	= x;
		d[5]	= y;
		d[10] = z;
		d[15] = 1.0;
		return d;
	}

	float *Trans(float x, float y, float z) {
		float *d = data;
		d[12] = x;
		d[13] = y;
		d[14] = z;
		d[15] = 1.0;
		return d;
	}

	float *Perspective(float fovy, float aspect, float fnear, float ffar) {
		float *d = data;
		Ident();
		float t = fnear * tan(fovy * 3.141592653 / 360.0);
		float r = t * aspect;
		float a = r * 2;
		float b = t * 2;
		float c = ffar - fnear;
		d[0]    = fnear * 2 / a;
		d[5]    = fnear * 2 / b;
		d[10]   = -(ffar + fnear) / c;
		d[11]   = -1;
		d[14]   = -(ffar * fnear * 2) / c;
		return d;
	};
	
	float *LookAt(vec &eye, vec &center, vec &up) {
		LookAt((float *)&eye, (float *)&center, (float *)&up);
		return (float *)this;
	}

	float *LookAt(float *eye, float *center, float *up) {
		float *d = data;
		Ident();
		float eyeX    = eye[0],    eyeY    = eye[1],    eyeZ    = eye[2];
		float upX     = up[0],     upY     = up[1],     upZ     = up[2];
		float centerX = center[0], centerY = center[1], centerZ = center[2];
		float x0, x1, x2, y0, y1, y2, z0, z1, z2, l;
		z0 = eyeX - center[0];
		z1 = eyeY - center[1];
		z2 = eyeZ - center[2];
		l = 1.0 / sqrt(z0 * z0 + z1 * z1 + z2 * z2);
		z0 *= l; z1 *= l; z2 *= l;
		
		//cross
		x0 = upY * z2 - upZ * z1;
		x1 = upZ * z0 - upX * z2;
		x2 = upX * z1 - upY * z0;
		l = sqrt(x0 * x0 + x1 * x1 + x2 * x2);
		if(!l){
		  x0 = 0; x1 = 0; x2 = 0;
		} else {
		  l = 1.0 / l;
		  x0 *= l; x1 *= l; x2 *= l;
		}
		
		//cross
		y0 = z1 * x2 - z2 * x1;
		y1 = z2 * x0 - z0 * x2;
		y2 = z0 * x1 - z1 * x0;
		l = sqrt(y0 * y0 + y1 * y1 + y2 * y2);
		if(!l) {
		  y0 = 0; y1 = 0; y2 = 0;
		} else {
		  l = 1 / l;
		  y0 *= l; y1 *= l; y2 *= l;
		}
		d[0]  = x0; d[1] = y0; d[2]  = z0; d[3]  = 0;
		d[4]  = x1; d[5] = y1; d[6]  = z1; d[7]  = 0;
		d[8]  = x2; d[9] = y2; d[10] = z2; d[11] = 0;
		d[12] = -(x0 * eyeX + x1 * eyeY + x2 * eyeZ);
		d[13] = -(y0 * eyeX + y1 * eyeY + y2 * eyeZ);
		d[14] = -(z0 * eyeX + z1 * eyeY + z2 * eyeZ);
		d[15] = 1;
		return d;
	}

	float *RotX(float a) {
		float *d = data;
		Ident();
		d[5]  =  cos(a);
		d[6]  = -sin(a);
		d[9]  =  sin(a);
		d[10] =  cos(a);
	}
	
	float *RotY(float a) {
		float *d = data;
		Ident();
		d[0]  =  cos(a);
		d[2]  =  sin(a);
		d[8]  = -sin(a);
		d[10] =  cos(a);
	}

	float *RotZ(float a) {
		float *d = data;
		Ident();
		d[0]  =  cos(a);
		d[1]  = -sin(a);
		d[4]  =  sin(a);
		d[5]  =  cos(a);
	}
	
	Matrix operator * (const Matrix &x) {
		Matrix ret;
		float  *a = (float *)&x;
		float  *b = (float *)this;
		float  *c = (float *)&ret;
		int idx = 0;
		for(int w = 0 ; w < 4; w++) {
			for(int j = 0 ; j < 4; j++) {
				c[idx] = 0;
				for(int i = 0 ; i < 4; i++) {
					c[idx] += a[i + w * 4] * b[i * 4 + j];
				}
				idx++;
			}
		}
		return ret;
	}
};

#endif _VECMAT_H_
