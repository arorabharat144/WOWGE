//====================================================================================================
// Constant Buffers
//====================================================================================================

#include "ConstantBuffers.fx"

cbuffer TransformBuffer : register(b3)
{
	matrix world;
}

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D displacementMap : register(t3);

Texture2D depthMap : register(t4);

SamplerState textureSampler : register(s0);

SamplerState depthSampler : register(s1);

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

//----------------------------------------------------------------------------------------------------

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 dirToLight : TEXCOORD0;
	float3 dirToView : TEXCOORD1;
	float2 texCoord : TEXCOORD2;
	float4 depthCoord : TEXCOORD3;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	//float4 position = input.position + (float4(input.normal, 0.0f) * displacementScale * displacement);
	//float4 position = input.position + (float4(input.normal, 0.0f));
	float4 position = input.position + (float4(input.normal, 0.0f) * displacement * 0.1f);

	float4 posWorld = mul(position, world);

	float4x4 wv = mul(world, cameraView[0]);
	float4x4 wvp = mul(wv, cameraProjection[0]);

	float4x4 wvLight = mul(world, cameraView[1]);
	float4x4 wvpLight = mul(wvLight, cameraProjection[1]);

	float4 posProj = mul(position, wvp);

	float3 normal = mul(float4(input.normal, 0.0f), world).xyz;
	float3 tangent = mul(float4(input.tangent, 0.0f), world).xyz;
	float3 binormal = cross(normal, tangent);

	float3 dirToLight = -normalize(lightDirection.xyz);
	float3 dirToView = normalize(cameraPosition[0].xyz - posWorld.xyz);

	output.position = posProj;
	output.normal = normal;
	output.tangent = tangent;
	output.binormal = binormal;
	output.dirToLight = dirToLight;
	output.dirToView = dirToView;
	output.texCoord = input.texCoord;
	output.depthCoord = mul(position, wvpLight);

	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VS_OUTPUT input) : SV_Target
{
	// Sample normal from normal map
	float4 normalColor = normalMap.Sample(textureSampler, input.texCoord);
	float3 sampledNormal = float3((2.0f * normalColor.xy) - 1.0f, normalColor.z);

	// Transform to world space
	float3 t = normalize(input.tangent);
	float3 b = normalize(input.binormal);
	float3 n = normalize(input.normal);
	float3x3 tbn = float3x3(t, b, n);
	float3 normal = n;

	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToView = normalize(input.dirToView);

	// Ambient
	float4 ambient = lightAmbient * materialAmbient;

	// Diffuse
	float d = saturate(dot(dirToLight, normal))/* : saturate(dot(dirToLight, n))*/;
	float4 diffuse = d * lightDiffuse * materialDiffuse;

	// Specular
	float3 h = normalize((dirToLight + dirToView));
	float s = saturate(dot(h, normal))/* : saturate(dot(h, n))*/;
	float4 specular = pow(s, materialPower) * lightSpecular * materialSpecular;

	float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
	float4 specularColor = specularMap.Sample(textureSampler, input.texCoord).rrrr;

	if (input.depthCoord.w <= 0.0f)
	{
		return ambient * diffuseColor;
	}

	//float4 finalColor = ((ambient + diffuse)) * 0.35f;
	float3 depthCoord = input.depthCoord.xyz / input.depthCoord.w;
	if (abs(depthCoord.x) > 1.0f || abs(depthCoord.y) > 1.0f)
	{
		return ((ambient + diffuse) * diffuseColor + (specular/* * specularColor*/));
	}
	float pixelDepth = depthCoord.z;
	float2 depthUV = (depthCoord.xy + 1.0f) * 0.5f;
	depthUV.y = 1.0f - depthUV.y;
	float sampledDepth = depthMap.Sample(depthSampler, depthUV).r + 0.01f;
	if (pixelDepth > sampledDepth)
	{
		return ambient * diffuseColor;
	}
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return ((ambient + diffuse) * diffuseColor + (specular/* * specularColor*/));
}