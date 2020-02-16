Texture2D diffuseTexture : register(t0);

SamplerState samplerState : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PSInput input, uint viewport : SV_ViewportArrayIndex) : SV_TARGET
{
    return diffuseTexture.Sample(samplerState, input.textureCoordinate);
}