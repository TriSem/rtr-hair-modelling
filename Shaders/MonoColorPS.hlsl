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

struct PSOutput
{
    float4 color1 : SV_Target0;
    float4 color2 : SV_Target1;
};

PSOutput main(PSInput input, uint viewport : SV_ViewportArrayIndex)
{
    PSOutput output;
    output.color1 = albedo;
    output.color2 = albedo;
    return output;
}