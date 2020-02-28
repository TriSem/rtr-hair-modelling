struct albedo
{
    
};

cbuffer Material : register(b1)
{
    
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PSInput input, uint viewport : SV_ViewportArrayIndex) : SV_TARGET
{
    return float4(0.4f, 0.4f, 0.4f, 8.0f);
}