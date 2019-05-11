//====================================================================================================
// Constant Buffers
//====================================================================================================

cbuffer CameraBuffer : register( b0 )
{
    matrix cameraView[2];
    matrix cameraProjection[2];
    vector cameraPosition[2];
}

cbuffer LightBuffer : register(b1)
{
	vector lightDirection;
	vector lightAmbient;
	vector lightDiffuse;
	vector lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	vector materialAmbient;
	vector materialDiffuse;
	vector materialSpecular;
	vector materialEmissive;
	float materialPower;
}