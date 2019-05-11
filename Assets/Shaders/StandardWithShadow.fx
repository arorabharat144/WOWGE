//====================================================================================================
// Filename:	StandardWithShadow.fx
// Description:	Standard shader.
//====================================================================================================

//====================================================================================================
// Constant Buffers
//====================================================================================================

cbuffer CameraBuffer : register(b0)
{
	matrix cameraView;
	matrix cameraProjection;
	vector cameraPosition;
	//float displacementFactor;
}

cbuffer LightBuffer : register(b1)
{
	vector lightDirection;
	vector lightAmbient;
	vector lightDiffuse;
	vector lightSpecular;
	//float displacementFactor;
}

cbuffer MaterialBuffer : register(b2)
{
	vector materialAmbient;
	vector materialDiffuse;
	vector materialSpecular;
	vector materialEmissive;
	float materialPower;
	//float displacementFactor;
}

cbuffer TransformBuffer : register(b3)
{
	matrix world;
	//float displacementFactor;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
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
    float3 normal	  : NORMAL;
	float3 tangent	  : TANGENT;
	float3 binormal	  : BINORMAL;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView   : TEXCOORD1;
	float2 texCoord	  : TEXCOORD2;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).r;
    float4 position = input.position + (float4(input.normal, 0.0f) * displacement * 0.1f); //0.1f is Displacement Factor
    float4 posWorld = mul(position, world);

	float4x4 wv = mul(world, cameraView);
	float4x4 wvp = mul(wv, cameraProjection);

    float4 posProj = mul(position, wvp);

    float3 normal = mul(float4(input.normal, 0.0f), world).xyz;
	float3 tangent = mul(float4(input.tangent, 0.0f), world).xyz;
	float3 binormal = cross(normal, tangent);

    float3 dirToLight = -normalize(lightDirection.xyz); //FOR DIFFUSE LIGHTING
    float3 dirToView = normalize(cameraPosition.xyz - posWorld.xyz); //FOR SPECULAR LIGHTING

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
	float4 normalColor = normalMap.Sample(textureSampler,input.texCoord);
	float3 sampledNormal = float3((2.0f*normalColor.xy) - 1.0f, normalColor.z);

	float3 t = normalize(input.tangent);
	float3 b = normalize(input.binormal);
	float3 n = normalize(input.normal);
    float3x3 tbn = float3x3(t, b, n);
	float3 normal = mul(sampledNormal, tbn);

	//Re-Normalize normals
    float3 dirToLight = normalize(input.dirToLight);
    float3 dirToView = normalize(input.dirToView);

	//Ambient
    float4 ambient = lightAmbient * materialAmbient;

	//Diffuse
    float d = saturate(dot(dirToLight, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

	//Specular
    float3 h = normalize(dirToLight + dirToView);
    float s = saturate(dot(h, normal));
    float4 specular = pow(s, materialPower) * lightSpecular * materialSpecular;

    float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 specularColor = specularMap.Sample(textureSampler, input.texCoord).rrrr;

    return (ambient + diffuse + specular);
}
