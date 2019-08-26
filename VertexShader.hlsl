cbuffer mvpMatrix
{
    float4x4 mvp;
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
    output.position = mul(mvp, float4(vertex.position, 1));
    output.normal = mul(mvp, vertex.normal);
    return output;
}