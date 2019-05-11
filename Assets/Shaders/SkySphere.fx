//====================================================================================================
// Filename:	Lighting.fx
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

Texture2D diffuseMap : register(t0);
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
	float2 texCoord	  : TEXCOORD2;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

    output.position = mul(input.position, world);
    output.position = mul(output.position, cameraView[0]);
    output.position = mul(output.position, cameraProjection[0]);

    output.texCoord = input.texCoord;

	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VS_OUTPUT input) : SV_Target
{
    return diffuseMap.Sample(textureSampler, input.texCoord);
}
