cbuffer mvpMatrix
{
    float4x4 mvp;
    float4x4 modelMatrix;
};

struct VSInput
{
    float3 position : SV_POSITION;
    float3 normal : NORMAL;
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