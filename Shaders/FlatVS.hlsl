struct VSInput
{
    float3 position : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : TANGENT;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput main(VSInput vertex)
{
    VSOutput output;
    output.position = float4(2 * vertex.textureCoordinate.r - 1, 2 * vertex.textureCoordinate.g - 1, 0.0f, 1.0f);
    return output;
}