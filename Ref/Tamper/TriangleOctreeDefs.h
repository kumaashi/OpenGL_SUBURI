#ifndef TRIANGLE_OCTREE_DEFS_H
#define TRIANGLE_OCTREE_DEFS_H

struct TriangleData
{
	float3 m_v0;
	float3 m_v1;
	float3 m_v2;
	float2 m_uv0;
	float2 m_uv1;
	float2 m_uv2;
	float m_materialIndex;
	float m_objectIndex;
	uint m_colour0;
	uint m_colour1;
	uint m_colour2;
	uint m_normal0;
	uint m_normal1;
	uint m_normal2;
};

#endif
