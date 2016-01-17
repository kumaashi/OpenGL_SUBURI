#ifndef SHADER_SHARED_STRUCTURES_H
#define SHADER_SHARED_STRUCTURES_H

#ifdef WIN32

namespace Demolition
{
#define float2 Core::Vector2
#define float3 Core::Vector3
#define float4 Core::Vector4
#define float4x4 Core::Matrix
#define uint uint32
#define ConstantBufferStruct struct


	namespace Fwd
	{
		namespace ShaderStructures
		{

#else

#define ConstantBufferStruct cbuffer

#endif

// due to misnamed properties, glossiness is really shininess and reflection blur is glossiness
struct MaterialProperties
{
	float4x4 m_proceduralTransform;

	float4 m_colour;
	float4 m_fresnelEnvelope;
	
	float m_luminosity;
	float m_diffuse;
	float m_glowiness;
	float m_glossiness;
	
	float m_specularPower;
	float m_wrapDiffuseCoeff;
	float m_colourFunctionWeight;
	float m_reflectionBlur;

	float m_proceduralParameter;
	float m_proceduralWeight;
	uint m_textureIndex;
	float m_density;

	uint m_reflectionEnabled;
	uint m_refractionEnabled;
	uint m_pad0;
	uint m_pad1;
};


struct DirectionalLight
{
	float4 m_colour;
	float4 m_shadowColour;
	float3 m_direction;				// view space	
	float m_shadowFadeAmount;
};

struct PointLight
{
	float3 m_position;				// view space
	float4 m_colour;
	float4 m_shadowColour;
	float m_shadowFadeAmount;
	float m_attenuationInner;
	float m_attenuationOuter;
};

struct SpotLight
{
	float3 m_position;				// view space
	float3 m_direction;				// view space
	float4 m_colour;
	float4 m_shadowColour;
	float m_shadowFadeAmount;
	float m_attenuationInner;
	float m_attenuationOuter;
	float m_coneAngleCosInner;
	float m_coneAngleCosOuter;
};

struct AreaLight
{
	float4 m_colour;
	float4 m_shadowColour;
	
	float4 m_position;
	float4 m_scale;
	
	float m_attenuationInner;	
	float m_attenuationOuter;
	float m_shapeBlend;
	float m_pad;
};


struct DeferredLight
{
	float4x4 m_worldTransform;
	float4x4 m_shadowTransform;
	float4x4 m_viewToLocalTransform;

	float4 m_colour;
	float4 m_shadowColour;
	
	float3 m_position;				// view space
	float m_attenuationInner;

	float3 m_direction;				// view space
	float m_attenuationOuter;
		
	float m_coneAngleCosInner;
	float m_coneAngleCosOuter;
	float m_shadowAmount;
	float m_shadowBluriness;

	uint m_lightType;
	float m_tanConeAngle;
	float m_scatterAmount;
	float m_pad0;
};

#define kMaxDeferredLights 128

struct DeferredLightBuffer
{
	DeferredLight m_lights[kMaxDeferredLights];
	uint m_numLights;
	uint m_deferredLightBufferPad[3];
};

struct DirectionalLightBuffer
{
	DirectionalLight m_directionalLights[kMaxDeferredLights];
	uint m_numDirectionalLights;
	uint m_directionalLightBufferPad[3];
};

#define kMaxNumAreaLights 4
struct AreaLightBuffer
{
	AreaLight m_arealights[kMaxNumAreaLights];
	uint m_numAreaLights;
	uint m_areaLightBufferPad0;
	uint m_areaLightBufferPad1;
	uint m_areaLightBufferPad2;
};


#define kMaxParticleMaterials 31

struct ParticleMaterial
{
	float4 m_colour;
	
	float m_diffuse;
	float m_emissiveness;
	float m_glossiness;
	float m_particleSize;

	float m_fadeInTime;
	float m_fadeOutTime;

	float m_pad0;
	float m_pad1;
};
struct ParticleMaterialBuffer
{
	ParticleMaterial m_particleMaterials[kMaxParticleMaterials];
	uint m_numParticleMaterials;
	uint m_particleMaterialBufferPad[3];
};

#define kMaxParticleMeshSkinBones 256
#define kMaxParticleMeshObjects 128
#define kMaxParticleMeshMaterials 256

struct ParticleMeshTransformBuffer
{
	float4x4 m_skinTransforms[kMaxParticleMeshSkinBones];
	float4x4 m_objectTransforms[kMaxParticleMeshObjects];
	float m_objectFertilizerTimes[kMaxParticleMeshObjects];
};

struct ParticleMeshMaterial
{
	float4 m_colour;
	float m_diffuse;
	float m_emissiveness;
	float m_glossiness;
	float m_colourFunctionWeight;
	float m_textureAmount;
	uint m_textureIndex;
	float m_pad0;
	float m_pad1;
};

struct ParticleMeshMaterialBuffer
{
	ParticleMeshMaterial m_materials[kMaxParticleMeshMaterials];
};

// 48 bytes
struct ParticleMeshVertex
{
	float3 m_position;
	uint m_colour;
	
	float2 m_uv;
	uint m_indices;
	uint m_weights;

	//uint m_normal;
	float m_fertilizerValue;
	uint m_objectIndex;
	uint m_materialIndex;
	uint m_originalVertexIndex;
};

struct ParticleMeshTriangle
{
	uint m_vertices[3];
	uint m_barycentric;
};


struct ParticleSkinElement
{
	uint m_indices;
	uint m_weights;
};

// Particle properties buffer member
struct ParticlePropertiesElement
{
	uint m_materialIndex;
	uint m_lineIndex;
};

#define PARTICLE_MATERIAL_MASK 0xff
#define PARTICLE_INITIAL_VELOCITY_SET 0x100
#define PARTICLE_HOLD_TO_PARENT 0x200

struct BvhNode
{
	float3 m_boundsMin;
	float3 m_boundsMax;
	uint m_leftNodeIndex;
	uint m_rightNodeIndex;
	uint m_passNextNode;
	uint m_failNextNode;
};
struct RaytracerTriangle
{
	float3 m_vertex;
	float3 m_edge0;
	float3 m_edge1;
	float3 m_normal;
	float2 m_uvs[3];
	uint m_materialIndex;
	uint m_bvhLeafIndex;
};
struct RaytracerVertex
{
	float3 m_position;
	float3 m_normal;
};


struct RigidBody
{
	float4 m_rotation;		// rotation quaternion
	float4 m_angularVelocity;// angular velocity quaternion
	float4 m_torque;		// torque

	float3 m_position;		// centre position
	float3 m_velocity;		// velocity
	float3 m_force;			// force
	float3 m_size;			// size

	uint m_vertexIndexOffset;// location of first vertex in buffer
	uint m_numVertices;		// num vertices in this rigid body
	uint m_active;			// rigid body is active
	float m_mass;			// mass of the rigid body
	
	/*
	float3 m_angularMomentum;// angular momentum
	
	float m_radius;			// radius
	int m_type;
	int m_pad;
	*/
};

struct GPUStaticRigidBody
{
	float3 m_boundsCentre;
	float3 m_boundsSize;
	float3 m_previousBoundsCentre;
	uint m_id;
	uint m_previousIndex;
	uint m_pad;
};

// Shader-only structures
#ifndef WIN32

struct GBufferOutput
{
	float4 ColourObjectId : SV_TARGET0;
	float4 NormalMaterialId : SV_TARGET1;
};


// Common shader code

// - RGBA 16-bit per component packed into a uint2 per texel
float4 UnpackRGBA16(uint2 e)
{
    return float4(f16tof32(e), f16tof32(e >> 16));
}
uint2 PackRGBA16(float4 c)
{
    return f32tof16(c.xy) | (f32tof16(c.zw) << 16);
}

// - RGBA 8-bit per component packed into a uint2 per texel
float4 UnpackRGBA8(uint e)
{
    return float4((float)(e & 0xff), (float)((e & 0xff00)>>8), (float)((e & 0xff0000)>>16), (float)((e & 0xff000000)>>24)) * (1.0f/255.0f);  
}
uint PackRGBA8(float4 c)
{
	float4 fc = saturate(c) * 255.0f;
	uint4 e = (uint4)fc;

	return e.x | (e.y << 8) | (e.z << 16) | (e.w << 24);
}


float EvaluateBezier1d(float t, float4 controlPoints)
{
	float invT = 1.0f-t;
	float coeff0 = invT*invT*invT;
	float coeff1 = invT*invT*t*3.0f;
	float coeff2 = invT*t*t*3.0f;
	float coeff3 = t*t*t;
	float4 coeffs = float4( coeff0,coeff1,coeff2,coeff3 );
	
	return dot(controlPoints,coeffs);
}

float SphericalGaussianApprox(float CosX, float ModifiedSpecularPower)
{
    return exp2(ModifiedSpecularPower * CosX - ModifiedSpecularPower);
}

// == 1/ln(2) * 6   (6 is SpecularPower of 5 + 1)
#define OneOnLN2_x6 8.656170 

float3 FresnelSchlickApprox(float3 SpecularColor,float3 E,float3 H)
{
    // In this case SphericalGaussianApprox(1.0f - saturate(dot(E, H)), OneOnLN2_x6) is equal to exp2(-OneOnLN2_x6 * x)
    return SpecularColor + (1.0f - SpecularColor) * exp2(-OneOnLN2_x6 * saturate(dot(E, H)));
}

float3 FresnelSchlick(float3 SpecularColor,float3 E,float3 H)
{
    return SpecularColor + (1.0f - SpecularColor) * pow(1.0f - saturate(dot(E, H)), 5);
}

float3 FresnelSchlickWithRoughness(float3 SpecularColor,float3 E,float3 N,float Gloss)
{
    return SpecularColor + (max(Gloss, SpecularColor) - SpecularColor) * pow(1 - saturate(dot(E, N)), 5);
}

float3 EvaluatePBLSpecular(float3 SpecularColor, float SpecularPower, float3 N, float3 L, float3 H)
{
	float dotNL = saturate(dot(N,L));
	return FresnelSchlick(SpecularColor, L, H) * ((SpecularPower + 2) / 8 ) * pow(saturate(dot(N, H)), SpecularPower) * dotNL;
}

#endif


#ifdef WIN32

		} //ShaderStructures
	} // Fwd
} // Demolition

#endif

#endif

