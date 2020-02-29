cbuffer mvpMatrix
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
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
    float4 position : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

VSOutput main(VSInput vertex)
{
    VSOutput output;
    float4x4 mv = (view, model);
    output.position = mul(mv, float4(vertex.position, 1));
    output.textureCoordinate = vertex.textureCoordinate;
    output.normal = mul(model, float4(vertex.normal, 0)).xyz;
    return output;
}