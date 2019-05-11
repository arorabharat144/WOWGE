//====================================================================================================
// Filename:	Lighting.fx
// Description:	Simple shader for texturing.
//====================================================================================================

//====================================================================================================
// Constant Buffers
//====================================================================================================

cbuffer ConstantBuffer : register( b0 )
{
	matrix world;
	matrix wvp;
	vector cameraPosition;
	vector lightDirection;
	vector lightAmbient;
    vector lightDiffuse;
    vector lightSpecular;
    vector materialAmbient;
    vector materialDiffuse;
    vector materialSpecular;
    vector materialEmissive;
    float materialPower;
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
    float3 normal	  : NORMAL;
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

    float4 position = input.position;
    float4 posWorld = mul(position, world);
    float4 posProj = mul(position, wvp);

    float3 normal = mul(float4(input.normal, 0.0f), world).xyz;

    float3 dirToLight = -normalize(lightDirection.xyz); //FOR DIFFUSE LIGHTING
    float3 dirToView = normalize(cameraPosition.xyz - posWorld.xyz); //FOR SPECULAR LIGHTING

    output.position = posProj;
    output.normal = normal;
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
	//Re-Normalize normals
    float3 normal = normalize(input.normal);
    float3 dirToLight = normalize(input.dirToLight);
    float3 dirToView = normalize(input.dirToView);

	//Ambient
    float4 ambient = lightAmbient * materialAmbient;

	//Diffuse
    float d = saturate(dot(dirToLight, normal));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

	//Specular
    float3 h = normalize(dirToLight + dirToView);
    float s = saturate(dot(h, normal));
    float4 specular = pow(s, materialPower) * lightSpecular * materialSpecular;

    float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);

	return diffuseColor * ((ambient + diffuse) * diffuseColor) + specular;
}
