//====================================================================================================
// Filename:	PostProcessing.fx
// Description:	Standard shader.
//====================================================================================================

//====================================================================================================
// Constant Buffers
//====================================================================================================

Texture2D postProcessingTexture : register(t0);
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
	int4 blendIndices : BLENDINDICES;
	float4 blendWeight : BLENDWEIGHT;
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

	output.position = input.position;
    output.texCoord = input.texCoord;

	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VS_OUTPUT input) : SV_Target
{
    //float4 sampledColor = postProcessingTexture.Sample(textureSampler, input.texCoord);
    float sampledColor = dot(postProcessingTexture.Sample(textureSampler, input.texCoord), float4(0.2, 0.6, 0.1, 0.1));
    //float grayscale = dot(sampledColor.rgb, float3(0.3, 0.59, 0.11));

    //sampledColor.r = grayscale;
    //sampledColor.g = grayscale;
    //sampledColor.b = grayscale;
    //sampledColor.a = 1.0f;


	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return float4(0.0f, sampledColor, 0.0f, 1.0f);
	//return sampledColor;
}
