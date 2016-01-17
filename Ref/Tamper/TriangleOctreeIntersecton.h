#ifndef TRIANGLE_OCTREE_INTERSECTION_H
#define TRIANGLE_OCTREE_INTERSECTION_H

#include "RaytracePrimitivesShader.h"
#include "ShaderRandom.h"
#include "TriangleOctreeDefs.h"


StructuredBuffer <uint> OctreeBuffer : OCTREEBUFFER;
StructuredBuffer <uint> OctreeParentBuffer : OCTREEPARENTBUFFER;
StructuredBuffer <uint2> TriangleLinkedListBuffer : TRIANGLELINKEDLISTBUFFER;
StructuredBuffer <TriangleData> TriangleBuffer : TRIANGLEBUFFER;
StructuredBuffer <uint> OctreeLevelCountBuffer : OCTREELEVELCOUNTBUFFER;
StructuredBuffer <uint> OctreeLevelOffsetBuffer : OCTREELEVELOFFSETBUFFER;
StructuredBuffer <uint2> VoxelDataBuffer : VOXELDATABUFFER;
Texture3D <uint> BrickIndexTexture : BRICKINDEXTEXTURE;

uint BaseOctreeLevelSize : BASEOCTREELEVELSIZE;
uint NumOctreeLevels : NUMOCTREELEVELS;
uint3 VoxelResolution : VOXELRESOLUTION;
uint BrickSize : BRICKSIZE;




float3 CalculateInverseRayDirection(float3 rayDirection)
{
	float eps = 0.00001f;
	rayDirection = abs(rayDirection) < eps ? (float3(sign(rayDirection)) * eps) : rayDirection;
	float3 rayInvDirection = float3(1.0f/rayDirection.x,1.0f/rayDirection.y,1.0f/rayDirection.z);
	return rayInvDirection;
}


uint RaytraceTriangles(uint listStart, float3 rayPosition, float3 rayDirection, uint skipTriangleIndex = 0xffffffff)
{
	uint hitTriangleIndex = 0xffffffff;
	float closestT = 100000000.0f;
	while(listStart != 0xffffffff)
	{
		uint triangleIndex = TriangleLinkedListBuffer[listStart].x;
	//	if(triangleIndex != skipTriangleIndex)
		{
			TriangleData tri = TriangleBuffer[triangleIndex];
			float t;
			bool hit = RayTriangleIntersection(t, tri.m_v0, tri.m_v1 - tri.m_v0, tri.m_v2 - tri.m_v0, rayPosition, rayDirection);
			[flatten] if(hit && t < closestT && triangleIndex != skipTriangleIndex)
			{
				closestT = t;
				hitTriangleIndex = triangleIndex;
			}
		}
		listStart = TriangleLinkedListBuffer[listStart].y;
	}
	return hitTriangleIndex; ;
}

// trace the triangles in cell, return closest hit with barycentric coordinates
uint RaytraceTrianglesBary(out float hitT, out float2 hitBarycentricCoords, uint listStart, float3 rayPosition, float3 rayDirection, uint skipTriangleIndex = 0xffffffff)
{
	uint hitTriangleIndex = 0xffffffff;
	float closestT = 100000000.0f;
	while(listStart != 0xffffffff)
	{
		uint triangleIndex = TriangleLinkedListBuffer[listStart].x;
	//	if(triangleIndex != skipTriangleIndex)
		{
			TriangleData tri = TriangleBuffer[triangleIndex];
			float t;
			float2 barycentricCoords;
			bool hit = RayTriangleIntersectionBary(t, barycentricCoords, tri.m_v0, tri.m_v1 - tri.m_v0, tri.m_v2 - tri.m_v0, rayPosition, rayDirection);
			
			[flatten] if(hit && t < closestT && triangleIndex != skipTriangleIndex)
			{
				closestT = t;
				hitTriangleIndex = triangleIndex;
				hitBarycentricCoords = barycentricCoords;
				hitT = t;
			}
		}
		listStart = TriangleLinkedListBuffer[listStart].y;
	}
	
	return hitTriangleIndex; ;
}

// trace the triangles in cell, exit if any hit
uint RaytraceTrianglesFirst(out float hitT, out float2 hitBarycentricCoords, uint listStart, float3 rayPosition, float3 rayDirection, uint skipTriangleIndex = 0xffffffff)
{
	uint hitTriangleIndex = 0xffffffff;
	while(listStart != 0xffffffff && hitTriangleIndex == 0xffffffff)
	{
		uint triangleIndex = TriangleLinkedListBuffer[listStart].x;
	
		TriangleData tri = TriangleBuffer[triangleIndex];
		float t;
		float2 barycentricCoords;
		bool hit = RayTriangleIntersectionBary(t, barycentricCoords, tri.m_v0, tri.m_v1 - tri.m_v0, tri.m_v2 - tri.m_v0, rayPosition, rayDirection);
		[flatten] if(hit && triangleIndex != skipTriangleIndex)
		{
			hitBarycentricCoords = barycentricCoords;
			hitT = t;				
			hitTriangleIndex = triangleIndex;
		}
		listStart = TriangleLinkedListBuffer[listStart].y;
	}
	
	return hitTriangleIndex; ;
}

// we assume that we are not at the entry cell here .. 
void InitialiseVoxelStepLevelDown(float3 leafPosition, inout float3 tMax, inout int3 rayCellPosition, int3 parentPos, float3 rayDelta, int3 rayDirectionSign, float3 rayDirection, float3 rayInvDirection)
{
	float3 fcellPos = float3(parentPos);
	
	float3 l1 = (fcellPos - leafPosition) * rayInvDirection;
	float3 l2 = (fcellPos+1.0f - leafPosition) * rayInvDirection;
	float3 lMin = min(l1,l2);
	
	float minT = max(max(lMin.x,lMin.y),lMin.z);
	
	//minT = min(minT, maxT);
	float3 cellEntryPoint = leafPosition;

	[flatten] if(all(leafPosition > fcellPos) && all(leafPosition < (fcellPos+1.0f)))
	{
	}
	else
	{
		cellEntryPoint += rayDirection * (minT + 0.001f);
	}
	cellEntryPoint -= fcellPos;
	cellEntryPoint *= 2.0f;
	
	rayCellPosition = cellEntryPoint >= 0.9999f;
	tMax = frac(cellEntryPoint);
	tMax = rayDirectionSign > 0 ? 1.0f-tMax : tMax;
	tMax *= rayDelta;
}

void StepVoxel(inout float3 tMax, inout int3 rayCellPosition, int3 rayDirectionSign, float3 rayDelta, float3 rayDirection)
{
	[flatten] if(tMax.x < tMax.y && tMax.x < tMax.z)
	{
		rayCellPosition.x += rayDirectionSign.x;
		tMax.x += rayDelta.x;
	}
	else if(tMax.y < tMax.z)
	{
		rayCellPosition.y += rayDirectionSign.y;
		tMax.y += rayDelta.y;
	}
	else
	{
		rayCellPosition.z += rayDirectionSign.z;
		tMax.z += rayDelta.z;
	}
}


uint RayOctreeTest(float3 rayPosition, float3 rayDirection)
{		
	float eps = 0.00001f;
	if(abs(rayDirection.x) < eps)
		rayDirection.x = rayDirection.x < 0 ? -eps : eps;
	if(abs(rayDirection.y) < eps)
		rayDirection.y = rayDirection.y < 0 ? -eps : eps;
	if(abs(rayDirection.z) < eps)
		rayDirection.z = rayDirection.z < 0 ? -eps : eps;
	rayDirection = normalize(rayDirection);
		
	float3 rayInvDirection = float3(1.0f/rayDirection.x,1.0f/rayDirection.y,1.0f/rayDirection.z);


	float3 originalRayPosition = rayPosition;
	
	float3 leafPosition = rayPosition * float(BaseOctreeLevelSize);
	
	int3 rayDirectionSign = sign(rayDirection);
	float3 rayDelta = abs(rayInvDirection);
	
	// find smallest component that steps the voxel
	int3 rayCellPosition = int3(leafPosition);
	float3 tMax = frac(leafPosition);
	tMax = rayDirectionSign > 0 ? 1.0f-tMax : tMax;
	tMax *= rayDelta;
	
	// stack structures
	uint levelIndex = 0;
	int iterations = 0;
	
	int4 stackData[8];
	float3 stackTmax[8];
	uint parentCellIndex = 0;
	int3 parentPos = 0;
	int levelBoundMax = BaseOctreeLevelSize;
			
	[loop] while(iterations < 200)
	{		
		uint cellIndex;
		[flatten] if(levelIndex > 0)
		{
			// pick a child level cell
			cellIndex = rayCellPosition.x | (rayCellPosition.y << 1) | (rayCellPosition.z << 2);
			cellIndex += parentCellIndex;
		}
		else
		{
			// pick a base level cell
			cellIndex = rayCellPosition.x + rayCellPosition.y * BaseOctreeLevelSize + rayCellPosition.z * BaseOctreeLevelSize * BaseOctreeLevelSize;
		}
			
		cellIndex = OctreeBuffer[cellIndex];
			
		[branch] if(cellIndex != 0xffffffff)
		{
			// step down a level
			if(levelIndex < NumOctreeLevels)
			{
				stackData[levelIndex] = int4(rayCellPosition, parentCellIndex);
				stackTmax[levelIndex] = tMax;
				parentCellIndex = cellIndex;
				parentPos = (parentPos << 1) + rayCellPosition;
				levelBoundMax = 2;
				
				InitialiseVoxelStepLevelDown(leafPosition * float(1<<levelIndex), tMax, rayCellPosition, parentPos, rayDelta, rayDirectionSign, rayDirection, rayInvDirection);
				
				++levelIndex;
			}
			else
			{
				// return the index of the first triangle
				return TriangleLinkedListBuffer[cellIndex].x;
			}
		}
		else 
		{
			StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
		}
	
		[loop] while((any(rayCellPosition < 0) || any(rayCellPosition >= levelBoundMax)) && iterations < 200 )
		{
			// step up a level
			if(levelIndex > 0)
			{
				--levelIndex ;
				rayCellPosition = stackData[levelIndex].xyz;
				parentCellIndex = stackData[levelIndex].w;
				tMax = stackTmax[levelIndex];
				parentPos >>= 1;
				if(levelIndex == 0)
				{
					parentPos = 0;
					levelBoundMax = BaseOctreeLevelSize;
				}
			
				// move the voxel to the next position on the level
				StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
				
				levelBoundMax = levelIndex > 0 ? 2 : BaseOctreeLevelSize;
			}
			else 
				return 0xffffffff;
		}
				
		++iterations;
	}
	return 0xffffffff;
}



uint RayOctreeEarlyTriangleTest(float3 rayPosition, float3 rayDirection, uint earlyCellCount)
{		
	float eps = 0.001f;
	if(abs(rayDirection.x) < eps)
		rayDirection.x = rayDirection.x < 0 ? -eps : eps;
	if(abs(rayDirection.y) < eps)
		rayDirection.y = rayDirection.y < 0 ? -eps : eps;
	if(abs(rayDirection.z) < eps)
		rayDirection.z = rayDirection.z < 0 ? -eps : eps;
	//rayDirection = normalize(rayDirection);
	
	rayPosition = saturate(rayPosition);
		
	float3 rayInvDirection = float3(1.0f/rayDirection.x,1.0f/rayDirection.y,1.0f/rayDirection.z);

	float3 originalRayPosition = rayPosition;
	
	float3 leafPosition = rayPosition * float(BaseOctreeLevelSize);
	
	int3 rayDirectionSign = sign(rayDirection);
	float3 rayDelta = abs(rayInvDirection);
	
	// find smallest component that steps the voxel
	int3 rayCellPosition = int3(leafPosition);
	int3 initialRayCellPosition = rayPosition * float(BaseOctreeLevelSize << NumOctreeLevels);
	float3 tMax = frac(leafPosition);
	tMax = rayDirectionSign > 0 ? 1.0f-tMax : tMax;
	tMax *= rayDelta;
	
	// stack structures
	uint levelIndex = 0;
	int iterations = 0;
	
	int4 stackData[8];
	float3 stackTmax[8];
	uint parentCellIndex = 0;
	int3 parentPos = 0;
	int levelBoundMax = BaseOctreeLevelSize;
			
	[loop] while(iterations < 200)
	{		
		uint cellIndex;
		[flatten] if(levelIndex > 0)
		{
			// pick a child level cell
			cellIndex = rayCellPosition.x | (rayCellPosition.y << 1) | (rayCellPosition.z << 2);
			cellIndex += parentCellIndex;
		}
		else
		{
			// pick a base level cell
			cellIndex = rayCellPosition.x + rayCellPosition.y * BaseOctreeLevelSize + rayCellPosition.z * BaseOctreeLevelSize * BaseOctreeLevelSize;
		}
			
		cellIndex = OctreeBuffer[cellIndex];
			
		[branch] if(cellIndex != 0xffffffff)
		{
			// step down a level
			if(levelIndex < NumOctreeLevels)
			{
				stackData[levelIndex] = int4(rayCellPosition, parentCellIndex);
				stackTmax[levelIndex] = tMax;
				parentCellIndex = cellIndex;
				parentPos = (parentPos << 1) + rayCellPosition;
				levelBoundMax = 2;
				
				InitialiseVoxelStepLevelDown(leafPosition * float(1<<levelIndex), tMax, rayCellPosition, parentPos, rayDelta, rayDirectionSign, rayDirection, rayInvDirection);
				
				++levelIndex;
			}
			else
			{
				int3 curPos = (parentPos<<1) + rayCellPosition;
				int3 dif = abs(curPos - initialRayCellPosition);
				[branch] if(dot(dif,dif) > (earlyCellCount*earlyCellCount))
				{
					return TriangleLinkedListBuffer[cellIndex].x;
				}
				else
				{
					uint triHit = RaytraceTriangles(cellIndex, originalRayPosition, rayDirection);
					if(triHit != 0xffffffff)
						return triHit;
					StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
				}
			}
		}
		else 
		{
			StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
		}
	
		[loop] while((any(rayCellPosition < 0) || any(rayCellPosition >= levelBoundMax)) && iterations < 200 )
		{
			// step up a level
			if(levelIndex > 0)
			{
				--levelIndex ;
				rayCellPosition = stackData[levelIndex].xyz;
				parentCellIndex = stackData[levelIndex].w;
				tMax = stackTmax[levelIndex];
				parentPos >>= 1;
				if(levelIndex == 0)
				{
					parentPos = 0;
					levelBoundMax = BaseOctreeLevelSize;
				}
			
				// move the voxel to the next position on the level
				StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
				
				levelBoundMax = levelIndex > 0 ? 2 : BaseOctreeLevelSize;
			}
			else 
				return 0xffffffff;
		}
				
		++iterations;
	}
	return 0xffffffff;
}



uint RayOctreeTriangleTest(float3 rayPosition, float3 rayDirection, uint skipTriangleIndex = 0xffffffff)
{		

	float eps = 0.00001f;
	if(abs(rayDirection.x) < eps)
		rayDirection.x = rayDirection.x < 0 ? -eps : eps;
	if(abs(rayDirection.y) < eps)
		rayDirection.y = rayDirection.y < 0 ? -eps : eps;
	if(abs(rayDirection.z) < eps)
		rayDirection.z = rayDirection.z < 0 ? -eps : eps;
	rayDirection = normalize(rayDirection);
		
	float3 rayInvDirection = float3(1.0f/rayDirection.x,1.0f/rayDirection.y,1.0f/rayDirection.z);

	float3 originalRayPosition = rayPosition;
	
	float3 leafPosition = rayPosition * float(BaseOctreeLevelSize);
	
	int3 rayDirectionSign = sign(rayDirection);
	float3 rayDelta = abs(rayInvDirection);
	
	// find smallest component that steps the voxel
	int3 rayCellPosition = int3(leafPosition);
	float3 tMax = frac(leafPosition);
	tMax = rayDirectionSign > 0 ? 1.0f-tMax : tMax;
	tMax *= rayDelta;
	
	// stack structures
	uint levelIndex = 0;
	int iterations = 0;
	
	int4 stackData[8];
	float3 stackTmax[8];
	uint parentCellIndex = 0;
	int3 parentPos = 0;
	int levelBoundMax = BaseOctreeLevelSize;

	uint hitTriangleResult = 0xffffffff;
			
	[loop] while(iterations < 200)
	{		
		uint cellIndex;
		[flatten] if(levelIndex > 0)
		{
			// pick a child level cell
			cellIndex = rayCellPosition.x | (rayCellPosition.y << 1) | (rayCellPosition.z << 2);
			cellIndex += parentCellIndex;
		}
		else
		{
			// pick a base level cell
			cellIndex = rayCellPosition.x + rayCellPosition.y * BaseOctreeLevelSize + rayCellPosition.z * BaseOctreeLevelSize * BaseOctreeLevelSize;
		}
			
		cellIndex = OctreeBuffer[cellIndex];
			
		[branch] if(cellIndex != 0xffffffff)
		{
			// step down a level
			if(levelIndex < NumOctreeLevels)
			{
				stackData[levelIndex] = int4(rayCellPosition, parentCellIndex);
				stackTmax[levelIndex] = tMax;
				parentCellIndex = cellIndex;
				parentPos = (parentPos << 1) + rayCellPosition;
				levelBoundMax = 2;
				
				InitialiseVoxelStepLevelDown(leafPosition * float(1<<levelIndex), tMax, rayCellPosition, parentPos, rayDelta, rayDirectionSign, rayDirection, rayInvDirection);
				
				++levelIndex;
			}
			else
			{
			//	return TriangleLinkedListBuffer[cellIndex].x;

				uint triHit = RaytraceTriangles(cellIndex, originalRayPosition, rayDirection, skipTriangleIndex);
				if(triHit != 0xffffffff)
				{
					hitTriangleResult = triHit;
					iterations = 1000000;
				}

				StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
			}
		}
		else 
		{
			StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
		}
	
		while((any(rayCellPosition < 0) || any(rayCellPosition >= levelBoundMax)) && iterations < 200 )
		{
			// step up a level
			if(levelIndex > 0)
			{
				--levelIndex ;
				rayCellPosition = stackData[levelIndex].xyz;
				parentCellIndex = stackData[levelIndex].w;
				tMax = stackTmax[levelIndex];
				parentPos >>= 1;

				[flatten] if(levelIndex == 0)
				{
					parentPos = 0;
					levelBoundMax = BaseOctreeLevelSize;
				}
			
				// move the voxel to the next position on the level
				StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);
				
				levelBoundMax = levelIndex > 0 ? 2 : BaseOctreeLevelSize;
			}
			else 
			{
				iterations = 1000000;
			}
		}
				
		++iterations;
	}
	return hitTriangleResult;
}



uint RayVoxelTriangleTest(float3 rayPosition, float3 rayDirection, uint skipTriangleIndex = 0xffffffff)
{		
	float eps = 0.00001f;
	if(abs(rayDirection.x) < eps)
		rayDirection.x = rayDirection.x < 0 ? -eps : eps;
	if(abs(rayDirection.y) < eps)
		rayDirection.y = rayDirection.y < 0 ? -eps : eps;
	if(abs(rayDirection.z) < eps)
		rayDirection.z = rayDirection.z < 0 ? -eps : eps;
	rayDirection = normalize(rayDirection);
		
	float3 rayInvDirection = float3(1.0f/rayDirection.x,1.0f/rayDirection.y,1.0f/rayDirection.z);

	float3 originalRayPosition = rayPosition;
	
	float3 leafPosition = rayPosition * float3(VoxelResolution);
	
	int3 rayDirectionSign = sign(rayDirection);
	float3 rayDelta = abs(rayInvDirection);
	
	// find smallest component that steps the voxel
	int3 rayCellPosition = int3(leafPosition);
	float3 tMax = frac(leafPosition);
	tMax = rayDirectionSign > 0 ? 1.0f-tMax : tMax;
	tMax *= rayDelta;

	int iterations = 0;
	uint hitTriangleResult = 0xffffffff;
	[loop] while(iterations < 300)
	{
		uint cellIndex = rayCellPosition.x + rayCellPosition.y * VoxelResolution.x + rayCellPosition.z * VoxelResolution.z * VoxelResolution.z;
		uint octreeTriangleIndex = OctreeBuffer[cellIndex];

		[branch] if(octreeTriangleIndex != 0xffffffff)
		{
			uint triHit = RaytraceTriangles(octreeTriangleIndex, originalRayPosition, rayDirection, skipTriangleIndex);
			if(triHit != 0xffffffff)
			{
				hitTriangleResult = triHit;
				iterations = 1000000;
			}
		}
		StepVoxel(tMax, rayCellPosition, rayDirectionSign, rayDelta, rayDirection);

		if(any(rayCellPosition < 0) || any(rayCellPosition >= VoxelResolution))
		{
			iterations = 1000000;
		}
		++iterations;
	}
	
	return hitTriangleResult;
}



uint RayBrickMapTest(float3 rayPosition, float3 rayDirection)
{	
	static const uint3 totalSize = VoxelResolution * BrickSize;
	static const float fBrickSize = float(BrickSize);
	static const uint brickIndexScale = BrickSize * BrickSize * BrickSize;
		
	float eps = 0.00001f;
	float3 rayInvDirection = CalculateInverseRayDirection(rayDirection);	
	float3 rayVoxelPosition = rayPosition * float3(totalSize);
	
	int iterations = 0;
	uint hitTriangleResult = 0xffffffff;
		
	[loop] while(iterations < 600)
	{
		int3 brickPos = int3(rayPosition * float3(VoxelResolution));
		uint brickIndex = BrickIndexTexture.Load(uint4(brickPos,0));
				
		[branch] if(brickIndex != 0xffffffff )
		{
			int3 rayCellPosition = int3(rayPosition * float3(totalSize));
			
			// calculate brick index
			uint3 brickSubPos = rayCellPosition & (BrickSize-1);
			uint brickCellAddress = brickSubPos.x + brickSubPos.y * BrickSize + brickSubPos.z * BrickSize * BrickSize;
			brickCellAddress += brickIndex * brickIndexScale;
			
			uint octreeTriangleIndex = OctreeBuffer[brickCellAddress];
			
			[branch] if(octreeTriangleIndex != 0xffffffff)
			{				
				float triT;
				float2 triBary;
				hitTriangleResult = RaytraceTriangles(octreeTriangleIndex, rayPosition, rayDirection);
				[flatten] if(hitTriangleResult != 0xffffffff)
				{
					iterations = 1000000;				
				}
			}
		}
				
		float3 curVoxelSize = brickIndex == 0xffffffff ? float3(VoxelResolution) : float3(totalSize);
		
		float3 rayPosScaled = rayPosition * curVoxelSize; 
		rayPosScaled = frac(rayPosScaled);	
		rayPosScaled = sign(rayDirection) > 0 ? 1.0f-rayPosScaled : rayPosScaled;
		float3 voxelSize = 1.0f/curVoxelSize;
		
		float3 tmaxScaled = rayPosScaled * abs(rayInvDirection);
		static const float rayEps = 0.00005f;
		[flatten] if(tmaxScaled.x < tmaxScaled.y && tmaxScaled.x < tmaxScaled.z)
		{
			rayPosition += rayDirection * ((abs(rayInvDirection.x) * voxelSize.x * rayPosScaled.x));// + rayEps);
		}
		else 
		[flatten] if(tmaxScaled.y < tmaxScaled.z)
		{
			rayPosition += rayDirection * ((abs(rayInvDirection.y) * voxelSize.y * rayPosScaled.y));// + rayEps);
		}
		else
		{
			rayPosition += rayDirection * ((abs(rayInvDirection.z) * voxelSize.z * rayPosScaled.z));// + rayEps);
		}
		// small epsilon to make sure we bump across the cell boundary. otherwise there's a danger that moving the ray forward doesnt actually move it into the next cell.
		rayPosition += rayDirection * rayEps;
			
		[flatten] if(any(rayPosition < 0) || any(rayPosition >= 1.0f))
		{
			iterations = 1000000;
		}
		
		++iterations;
	}
	
	return hitTriangleResult;
}


#endif


