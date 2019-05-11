//====================================================================================================
// Filename:	DepthMap.fx
// Description:	Standard shader.
//====================================================================================================

//====================================================================================================
// Constant Buffers
//====================================================================================================

#include "ConstantBuffers.fx"

cbuffer TransformBuffer : register(b3)
{
    matrix world;
}

cbuffer BoneBuffer : register(b5)
{
	matrix boneTransforms[100];
}

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

Texture2D depthMap : register(t4);
SamplerState textureSampler : register(s1);

//====================================================================================================
// Structs
//====================================================================================================

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 texCoord	: TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeight : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 position	  : SV_POSITION;
};

struct VS_DEBUG_INPUT
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_DEBUG_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f)
		return Identity;

	matrix transform;
	transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

//float4 position = input.position + (float4(input.normal, 0.0f)); //0.1f is Displacement Factor
   

    float4x4 wv = mul(world, cameraView[1]);
    float4x4 wvp = mul(wv, cameraProjection[1]);

	matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeight);

	float4 posBone = input.position;
	float4 posLocal = mul(posBone, boneTransform);
    float4 posProj = mul(posLocal, wvp);

    output.position = posProj;

    return output;
}

VS_DEBUG_OUTPUT VSDebug(VS_DEBUG_INPUT input)
{
    VS_DEBUG_OUTPUT output = (VS_DEBUG_OUTPUT)0;
    output.position = input.position;
    output.texCoord = input.texCoord;
    return output;
}

float4 PSDebug(VS_DEBUG_OUTPUT input) : SV_Target
{
    float depth = depthMap.Sample(textureSampler, input.texCoord).r;
    float near = 25.0f;
    float far = 500.0f;
    float linearDepth = (2.0f * near) / (far + near - depth * (far - near));
    return linearDepth;
};