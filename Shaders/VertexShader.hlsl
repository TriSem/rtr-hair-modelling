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
    float3 bitangent : TANGENT;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput main(VSInput vertex)
{
    VSOutput output;
    float4x4 mvp = mul(model, view);
    mvp = mul(mvp, projection);
    output.position = mul(mvp, float4(vertex.position, 1.0f));
    //output.normal = normalize(mul(float4(vertex.normal, 0.0f), modelMatrix));
    return output;
}