cbuffer Material : register(b1)
{
    float4 albedo;
    float roughness;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PSInput input, uint viewport : SV_ViewportArrayIndex) : SV_TARGET
{
    return albedo;
}