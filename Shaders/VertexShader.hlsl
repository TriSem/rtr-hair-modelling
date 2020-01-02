cbuffer mvpMatrix
{
    float4x4 mvp;
    float4x4 modelMatrix;
};

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
    float3 normal : NORMAL;
};

VSOutput main(VSInput vertex)
{
    VSOutput output;
    output.position = mul(mvp, float4(vertex.position, 1.0f));
    output.normal = normalize(mul(float4(vertex.normal, 0.0f), modelMatrix));
    return output;
}