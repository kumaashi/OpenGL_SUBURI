#ifndef RAYTRACE_TRIANGLE_MESH_SHADER_H
#define RAYTRACE_TRIANGLE_MESH_SHADER_H

bool RayPlaneIntersection(out float t, float4 plane, float3 rayOrigin, float3 rayDir)
{
	float numerator = -(dot(plane.xyz, rayOrigin) + plane.w);
	float denominator = dot(plane.xyz, rayDir);
	
	if(denominator <= 0.00001f)
	{
		return false;
	}
	t = numerator/denominator;
	return true;
}


bool RayBoxIntersection(out float tmin, out float tmax, float3 boxMin, float3 boxMax, float3 rayOrigin, float3 rayDirInv)
{
	float3 l1 = (boxMin - rayOrigin) * rayDirInv;
	float3 l2 = (boxMax - rayOrigin) * rayDirInv;

	float3 lMin = min(l1,l2);
	float3 lMax = max(l1,l2);

	tmin = max(max(lMin.x,lMin.y),lMin.z);
	tmax = min(min(lMax.x,lMax.y),lMax.z);

	return ((tmax >= tmin) && (tmax >= 0.0f));
}


bool RaySphereIntersection(out float t, float3 sphereCentre, float sphereRadius, float3 rayOrigin, float3 rayDir)
{
	float3 sr = rayOrigin - sphereCentre;
	float b =  dot(sr,rayDir);
	float c = dot(sr,sr) - (sphereRadius*sphereRadius);
	float d = b*b - c;

	if (d > 0) 
	{
		float e = sqrt(d);
		float t0 = -b-e;
		if(t0 < 0)
			t = -b+e;
		else
			t = min(-b-e,-b+e);

		return true;
	}
	return false;
}


/*
// the classic ray triangle intersection: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
bool RayTriangleIntersection(out float t, float3 v0, float3 edge1, float3 edge2, float3 rayOrigin, float3 rayDir)		 
{  
	float3 tvec = rayOrigin - v0;  
	float3 pvec = cross(rayDir, edge2);  
	float  det  = dot(edge1, pvec);  

	det = 1.0f / det;  
	float u = dot(tvec, pvec) * det;  
	if (u < 0.0f || u > 1.0f)  
		return false;  

	float3 qvec = cross(tvec, edge1);  
	float v = dot(rayDir, qvec) * det;  

	if (v < 0.0f || (u + v) > 1.0f)  
		return false;  

	t = dot(edge2, qvec) * det;  
	return t >= 0.0f;
}  
*/
bool RayTriangleIntersection(out float t, float3 v0, float3 edge1, float3 edge2, float3 rayOrigin, float3 rayDir)		 
{  
	float3 tvec = rayOrigin - v0;  
	float3 pvec = cross(rayDir, edge2);  
	float  det  = dot(edge1, pvec);  

	det = 1.0f / det;  
	float u = dot(tvec, pvec) * det;  
	float3 qvec = cross(tvec, edge1); 

	float v = dot(rayDir, qvec) * det;  
	
	t = dot(edge2, qvec) * det;  
	
	return u >= 0.0f && u <= 1.0f && v >= 0.0f && (u+v) <= 1.0f && t >= 0.0f;
}  

// the classic ray triangle intersection: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
bool RayTriangleIntersectionBary(out float t, out float2 barycentricCoords, float3 v0, float3 edge1, float3 edge2, float3 rayOrigin, float3 rayDir)		 
{  
	float3 tvec = rayOrigin - v0;  
	float3 pvec = cross(rayDir, edge2);  
	float3 qvec = cross(tvec, edge1);  
	float  det  = dot(edge1, pvec);  
	
	det = 1.0f / det;  
	float u = dot(tvec, pvec) * det;  
	float v = dot(rayDir, qvec) * det;  
	t = dot(edge2, qvec) * det;  
	
	barycentricCoords = float2(u,v);
	
	bool rslt = !(u < 0.0f || u > 1.0f || v < 0.0f || (u + v) > 1.0f || t < 0.0f) ;
	return rslt;
}  



float3 GetClosestPointOnTriangle(float3 sourcePosition, float3 triV0, float3 triV1, float3 triV2, out float bu, out float bv)
{
    float3 edge0 = triV1 - triV0;
    float3 edge1 = triV2 - triV0;
    float3 v0 = triV0 - sourcePosition;

    float a = dot( edge0, edge0 );
    float b = dot( edge0, edge1 );
    float c = dot( edge1, edge1 );
    float d = dot( edge0, v0 );
    float e = dot( edge1, v0 );

    float det = a*c - b*b;
    float s = b*e - c*d;
    float t = b*d - a*e;

    if ( s + t < det )
    {
   		float ns = clamp( -d/a, 0.f, 1.f );
   		float nt = clamp( -e/c, 0.f, 1.f );
   
        if ( s < 0.f )
        {
        	bu = (t < 0 && d < 0) ? ns : 0;
        	bv = (t < 0 && d < 0) ? 0 : nt;          
        }
        else if ( t < 0.f )
        {
            bu = ns;
            bv = 0.f;
        }
        else
        {
            float invDet = 1.f / det;
            bu = s * invDet;
            bv = t * invDet;
        }
    }
    else
    {
    	float denom = 1.0f / ( a-2*b+c);
    	float numer = c+e-b-d;
    	float ns = clamp( numer*denom, 0.f, 1.f );
    	float ec = clamp( -e/c, 0.f, 1.f );
    
        if ( s < 0.f )
        {
        	bu = (c+e) > (b+d) ? ns : 0;
        	bv = (c+e) > (b+d) ? 1-ns : ec;
        }
        else if ( t < 0.f )
        {
            if ( a+d > b+e )
            {
                bu = ns;
                bv = 1-ns;
            }
            else
            {
                bu = ec;
                bv = 0.f;
            }
        }
        else
        {
            bu = ns;
            bv = 1 - ns;
        }
    }
	
    return triV0 + edge0 * bu + edge1 * bv;
}





bool PlaneBoxOverlap(float3 normal, float3 vert, float3 maxbox)	
{
	float3 v0 = -maxbox - vert;
	float3 v1 = maxbox - vert;
	
	float3 vmin = normal > 0.0f ? v0 : v1;
	float3 vmax = normal > 0.0f ? v1 : v0;
	
	if(dot(normal, vmin) > 0.0f || dot(normal, vmax) < 0.0f)
		return false;
	else
		return true;
}

//======================== X-tests ========================

#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0.y - b*v0.z;			       	   \
	p2 = a*v2.y - b*v2.z;			       	   \
	rad = fa * boxhalfsize.y + fb * boxhalfsize.z;   \
	if(min(p0,p2) > rad || max(p0,p2) < -rad) \
		return false; 

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0.y - b*v0.z;			           \
	p1 = a*v1.y - b*v1.z;			       	   \
	rad = fa * boxhalfsize.y + fb * boxhalfsize.z;   \
	if(min(p0,p1) > rad || max(p0,p1) < -rad) \
		return false;


//======================== Y-tests ========================

#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0.x + b*v0.z;		      	   \
	p2 = -a*v2.x + b*v2.z;	       	       	   \
	rad = fa * boxhalfsize.x + fb * boxhalfsize.z;   \
	if(min(p0,p2) > rad || max(p0,p2) < -rad) \
		return false;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0.x + b*v0.z;		      	   \
	p1 = -a*v1.x + b*v1.z;	     	       	   \
	rad = fa * boxhalfsize.x + fb * boxhalfsize.z;   \
	if(min(p0,p1) > rad || max(p0, p1) < -rad) \
		return false;



//======================== Z-tests ========================



#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1.x - b*v1.y;			           \
	p2 = a*v2.x - b*v2.y;			       	   \
	rad = fa * boxhalfsize.x + fb * boxhalfsize.y;   \
	if(min(p1,p2) > rad || max(p1, p2) < -rad) \
		return false;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0.x - b*v0.y;				   \
	p1 = a*v1.x - b*v1.y;			           \
    rad = fa * boxhalfsize.x + fb * boxhalfsize.y;   \
	if(min(p0, p1) > rad || max(p0, p1) < -rad) \
		return false;

bool TriBoxOverlap(float3 boxcenter,float3 boxhalfsize, float3 triVert0, float3 triVert1, float3 triVert2)
{
	// use separating axis theorem to test overlap between triangle and box 
	// need to test for overlap in these directions: 
	// 1) the {x,y,z}-directions (actually, since we use the AABB of the triangle we do not even need to test these) 
	// 2) normal of the triangle 
	// 3) crossproduct(edge from tri, {x,y,z}-direction).. this gives 3x3=9 more tests 

	float p0,p1,p2,rad;		
   
	 // move everything so that the boxcenter is in (0,0,0) 
	float3 v0 = triVert0 - boxcenter;
	float3 v1 = triVert1 - boxcenter;
	float3 v2 = triVert2 - boxcenter;

	// compute triangle edges 
	float3 e0 = v1 - v0;
	float3 e1 = v2 - v1;
	float3 e2 = v0 - v2;
  
	// Bullet 1: 
	// first test overlap in the {x,y,z}-directions 
	// find min, max of the triangle each direction, and test for overlap in that direction -- this is equivalent to testing a minimal AABB around the triangle against the AABB
	
	float3 axisMin = min(v0,min(v1,v2));
	float3 axisMax = max(v0,max(v1,v2));
	 
	if(any(axisMin > boxhalfsize) || any(axisMax < -boxhalfsize))
		return false;

	// Bullet 2: 
	//  test if the box intersects the plane of the triangle 
	//  compute plane equation of triangle: normal*x+d=0 
	float3 normal = cross(e0,e1);
    if(!PlaneBoxOverlap(normal,v0,boxhalfsize)) 
		return false;
   
	//  test the 9 tests first (this was faster) 
	float3 fe = abs(e0);
	AXISTEST_X01(e0.z, e0.y, fe.z, fe.y);
	AXISTEST_Y02(e0.z, e0.x, fe.z, fe.x);
	AXISTEST_Z12(e0.y, e0.x, fe.y, fe.x);

	fe = abs(e1);
	AXISTEST_X01(e1.z, e1.y, fe.z, fe.y);
	AXISTEST_Y02(e1.z, e1.x, fe.z, fe.x);
	AXISTEST_Z0(e1.y, e1.x, fe.y, fe.x);

	fe = abs(e2);
	AXISTEST_X2(e2.z, e2.y, fe.z, fe.y);
	AXISTEST_Y1(e2.z, e2.x, fe.z, fe.x);
	AXISTEST_Z12(e2.y, e2.x, fe.y, fe.x);

	return true;
}


#endif
