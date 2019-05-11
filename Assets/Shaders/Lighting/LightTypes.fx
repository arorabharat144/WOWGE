struct DirectionalLight
{
    vector ambient;
    vector diffuse;
    vector specular;

    float3 direction;

    float pad;
};

struct PointLight
{
    vector ambient;
    vector diffuse;
    vector specular;

    float3 position;
    float3 range;

    float pad;
};

struct SpotLight
{
    vector ambient;
    vector diffuse;
    vector specular;
    float3 position;
    float3 direction;
    float3 range;
    float  spot;
    float  attenuation;

    float  pad;
};