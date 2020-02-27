struct PSInput
{
    float4 position : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PSInput input, uint viewport : SV_ViewportArrayIndex) : SV_TARGET
{
    return float4(0.0f, 1.0f, 1.0f, 1.0f);
}