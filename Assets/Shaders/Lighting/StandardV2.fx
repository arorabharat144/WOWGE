//====================================================================================================
// Filename:	Lighting.fx
// Description:	Simple shader for texturing.
//====================================================================================================

#include "Material.fx"
#include "LightTypes.fx"

//====================================================================================================
// Constant Buffers
//====================================================================================================

cbuffer ConstantBuffer : register(b0)
{
    matrix world;
    matrix wvp;
    vector cameraPosition;
    Material material;
    float displacementFactor;
}

cbuffer LightBuffer : register(b1)
{
    DirectionalLight directionalLight;
    PointLight       pointLight;
    SpotLight        spotLight;
}
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
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
   float3 normal : NORMAL;
   float3 dirToLight : TEXCOORD0;
   float3 dirToView : TEXCOORD1;
   float2 texCoord : TEXCOORD2;
};

//====================================================================================================
// Vertex Shader
//====================================================================================================

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).r;
    float4 position = input.position + (float4(input.normal, 0.0f) * displacement * displacementFactor);
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
    float4 ambient = lightAmbient * material.ambient;

	//Diffuse
    float d = saturate(dot(dirToLight, normal));
    float4 diffuse = d * lightDiffuse * material.diffuse;

	//Specular
    float3 h = normalize(dirToLight + dirToView);
    float s = saturate(dot(h, normal));
    float4 specular = pow(s, material.power) * lightSpecular * material.specular * specularMap.Sample(textureSampler, input.texCoord).r;

    float4 diffuseColor = diffuseMap.Sample(textureSampler, input.texCoord);

    return ((ambient + diffuse) * diffuseColor) + specular;
}

void ComputeDirectionalLight(Material mat, DirectionalLight light, float3 normal, float3 toEye, out vector amb, out vector diff, out vector spec)
{
	// Initialize outputs.
    amb  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diff = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	// The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -normalize(light.direction);
	// Add ambient term.
    amb = mat.ambient * light.ambient;
	// Add diffuse and specular term, provided the surface is in
	// the line of site of the light.
    float diffuseFactor = saturate(dot(lightVec, normal));
	// Flatten to avoid dynamic branching.
	[flatten]

    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);
        diff = diffuseFactor * mat.diffuse * light.diffuse;
        spec = specFactor * mat.specular * light.specular;
    }
}