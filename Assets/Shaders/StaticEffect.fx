//====================================================================================================
// Filename:	FilmGrain.fx
// Description:	Standard shader.
//====================================================================================================

//====================================================================================================
// Constant Buffers
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
	float timer;
	float noiseAmount;
	int seed;
	float scanLinesAmount;
}

cbuffer SelecterBuffer : register(b1)
{
	vector shaderSelector;
}

Texture2D postProcessingTexture : register(t0);
SamplerState textureSampler : register(s0);

//====================================================================================================
// Structs
//====================================================================================================

struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD2;
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
	float noiseX = seed * timer * sin(input.texCoord.x * input.texCoord.y + timer);
noiseX = fmod(noiseX, 8) * fmod(noiseX, 4);

float distortX = fmod(noiseX, noiseAmount);
float distortY = fmod(noiseX, noiseAmount + 0.002f);

float2 distortTexCoord = float2(distortX, distortY);

float4 color = postProcessingTexture.Sample(textureSampler, input.texCoord + distortTexCoord);
color.a = 1.0f;

//ScanLines
//Sample the source
float4 textureScreen = postProcessingTexture.Sample(textureSampler, input.texCoord.xy);

//Make some noise
float x = input.texCoord.x * input.texCoord.y * timer * 1000.0f;
x = fmod(x, 13.0f) * fmod(x, 123.0f);
float dx = fmod(x, 0.01f);

//Add noise
float3 result = textureScreen.rgb + textureScreen.rgb * saturate(0.1f + dx.xxx * 100.0f);

//Get us a sine and cosine
float2 sc;
sincos(input.texCoord.y * 2048.0f, sc.x, sc.y);

//Add scanlines
result += textureScreen.rgb * float3(sc.x, sc.y, sc.x) * scanLinesAmount;

//result = lerp(textureScreen, result, saturate(noiseAmount));

//result.rgb = dot(result.rgb, float3(0.3f, 0.59f, 0.11f));

float3 combinedResult;
if (shaderSelector.x == 0.0f)
{
	combinedResult = dot(color.rgb, result);
}
else if (shaderSelector.x == 1.0f)
{
	combinedResult = cross(color.rgb, result);
}
else if (shaderSelector.x == 2.0f)
{
	combinedResult = (color + float4(result, 1.0f)) * 0.5f;
}
else if (shaderSelector.x == 3.0f)
{
	combinedResult = lerp(color.rgb, result, 100.0f);
}

return float4(combinedResult, 1.0f);
}