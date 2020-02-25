cbuffer mvpMatrix
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

struct HairProperties
{
    float length;
    float tangent;
    float bitangent;
};

struct VSInput
{
    float3 position : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
};

struct VSOutput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    HairProperties hairProperties : HAIR;
    float4x4 projection : MATRIX;
};

VSOutput main(VSInput vertex)
{
    VSOutput output;
    float4x4 mv = mul(view, model);
    output.position = mul(mv, float4(vertex.position, 1));
    output.normal = (float3) mul(mv, float4(vertex.normal, 0));
    output.tangent = (float3) mul(mv, float4(vertex.tangent, 0));
    output.bitangent = (float3) mul(mv, float4(vertex.bitangent, 0));
    output.hairProperties.length = 0.1;
    output.hairProperties.tangent = 0.8;
    output.hairProperties.bitangent = 0.8;
    output.projection = projection;
    return output;
}