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
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
	float2 texCoord	: TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
	float2 texCoord	  : TEXCOORD2;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    float4 position = input.position;
    float4 posWorld = mul(position, world);

    float4x4 wv = mul(world, cameraView[0]);
    float4x4 wvp = mul(wv, cameraProjection[0]);

    float4 posProj = mul(position, wvp);

    float3 normal = mul(float4(input.normal, 0.0f), world).xyz;
    float3 tangent = mul(float4(input.tangent, 0.0f), world).xyz;
    float3 binormal = cross(normal, tangent);

    float3 dirToLight = -normalize(lightDirection.xyz); //FOR DIFFUSE LIGHTING
    float3 dirToView = normalize(cameraPosition[0].xyz - posWorld.xyz);

    output.position = posProj;
    output.normal = normal;
    output.tangent = tangent;
    output.binormal = binormal;
    output.dirToLight = dirToLight;
    output.dirToView = dirToView;
    output.texCoord = input.texCoord;

    return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 t = normalize(input.tangent);
    float3 b = normalize(input.binormal);
    float3 n = normalize(input.normal);
    float3x3 tbn = float3x3(t, b, n);

	//Re-Normalize normals
    float3 dirToLight = normalize(input.dirToLight);
    float3 dirToView = normalize(input.dirToView);

	//Ambient
    float4 ambient = lightAmbient * materialAmbient;

	//Diffuse
    float d = saturate(dot(dirToLight, n));
    float4 diffuse = /*d * */lightDiffuse * materialDiffuse;

	//Specular
    float3 h = normalize(dirToLight + dirToView);
    float s = saturate(dot(h, n));
    float4 specular = pow(s, materialPower) * lightSpecular * materialSpecular;

    float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
    if (diffuseColor.x == 0.0f &&
		diffuseColor.y == 0.0f &&
		diffuseColor.z == 0.0f &&
		diffuseColor.w == 0.0f)
	{
		return ((ambient + diffuse) + specular);
	}

	return ((ambient + diffuse) * diffuseColor + specular);
}
