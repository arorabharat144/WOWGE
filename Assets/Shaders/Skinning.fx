//====================================================================================================
// Filename:	Skinning.fx
// Description:	Shader performing hardware skinning.
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
	matrix world;
	matrix wvpLight;
	float3 viewPosition;		// World space
	float3 lightDirection;		// World space
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float power;
}

cbuffer BoneConstantBuffer : register(b1)
{
	matrix boneTransforms[64];
}

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

struct VSInput
{
	float4 position		: POSITION;
	float3 normal 		: NORMAL;
	float3 tangent		: TANGENT;
	float4 color		: COLOR;
	float2 texcoord		: TEXCOORD;
	int4 blendIndices	: BLENDINDICES;
	float4 blendWeight	: BLENDWEIGHT;
};

struct VSOutput
{
	float4 position		: SV_POSITION;
	float3 worldPos		: TEXCOORD0;
	float3 normal		: TEXCOORD1;
	float3 tangent		: TEXCOORD2;
	float2 texcoord		: TEXCOORD3;
	float4 shadowCoord	: TEXCOORD4;
};

Texture2D DiffuseMap : register(t0);
Texture2D ShadowMap : register(t1);
SamplerState Sampler : register(s0);

//====================================================================================================
// Helpers
//====================================================================================================

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

VSOutput VS(VSInput input)
{
	VSOutput output = (VSOutput)0;

	matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeight);
	float4 posBone = input.position;
	float4 posLocal = mul(posBone, boneTransform);
	float4 posWorld = mul(posLocal, world);
	float4 posProj = mul(posLocal, wvp);
	output.worldPos = posWorld.xyz;
	output.position = posProj;

	// We need to convert a 3d vector to a homogenous vector
	// Using w = 0 means we don't care about translation.
	// The result of a 1by4 * 4by4 multiplication gives a 1by4
	// and we only need the xyz values so we use HLSL's swizzling
	// syntax to extract just the x, y, and z
	float3 normalBone = input.normal;
	float3 normalLocal = mul(float4(normalBone, 0.0f), boneTransform);
	output.normal = mul(float4(normalLocal, 0.0f), world).xyz;

	output.texcoord = input.texcoord;
	output.shadowCoord = mul(input.position, wvpLight);
	return output;
}

//====================================================================================================
// Pixel Shader
//====================================================================================================

float4 PS(VSOutput input) : SV_Target
{
	// Re-normalize normals
	float3 normal = normalize(input.normal);
	float3 dirToLight = -lightDirection;
	float3 dirToView = normalize(viewPosition - input.worldPos);

	// Ambient
	float4 ambientColor = lightAmbient * materialAmbient;

	// Diffuse
	float diffuse = saturate(dot(dirToLight, normal));
	float4 diffuseColor = diffuse * lightDiffuse * materialDiffuse;

	// Specular
	float3 halfVector = normalize((dirToLight + dirToView) * 0.5f);
	float specular = saturate(dot(halfVector, normal));
	float4 specularColor = pow(specular, power) * lightSpecular * materialSpecular;

	// Texture values
	float4 diffuseTexture = DiffuseMap.Sample(Sampler, input.texcoord);

	// Final color
	float4 finalColor = ((ambientColor + diffuseColor) * diffuseTexture) * 0.35f;
	if (input.shadowCoord.w >= 0.0f)
	{
		float3 shadowCoord = input.shadowCoord.xyz / input.shadowCoord.w;
		shadowCoord.x = (shadowCoord.x + 1.0f) * 0.5f;
		shadowCoord.y = (-shadowCoord.y + 1.0f) * 0.5f;
		float pixelDepth = shadowCoord.z;
		float sampledDepth = ShadowMap.Sample(Sampler, shadowCoord.xy).r + 0.0004f;
		if (pixelDepth < sampledDepth)
		{
			finalColor = ((ambientColor + diffuseColor) * diffuseTexture) + specularColor;
		}
	}
	return finalColor;
}
