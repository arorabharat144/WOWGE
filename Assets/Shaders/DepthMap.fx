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

Texture2D depthMap : register(t4);
SamplerState textureSampler : register(s0);

//====================================================================================================
// Structs
//====================================================================================================

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 texCoord	: TEXCOORD;
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

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

//float4 position = input.position + (float4(input.normal, 0.0f)); //0.1f is Displacement Factor
    float4 position = input.position;
    float4 posWorld = mul(position, world);

    float4x4 wv = mul(world, cameraView[1]);
    float4x4 wvp = mul(wv, cameraProjection[1]);

    float4 posProj = mul(position, wvp);

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