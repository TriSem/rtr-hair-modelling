cbuffer mvpMatrix
{
    float4x4 mvp;
    float4x4 modelMatrix;
};

struct VertexShaderInput
{
    float3 position : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : TANGENT;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
};

RasterizerState WireframeRasterizerState
{
    FillMode = Wireframe;
    CullMode = Back;
};

VertexShaderOutput VertexShader(VertexShaderInput vertex)
{
    VertexShaderOutput output;
    output.position = mul(float4(vertex.position, 1.0f), mvp);
    //output.normal = normalize(mul(float4(vertex.normal, 0.0f), modelMatrix));
    return output;
}

float4 PixelShader(float4 position : SV_POSITION) : SV_TARGET
{
    return float4(0.7f, 0.7f, 0.7f, 1.0f);
}

technique11 tech
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VertexShader()));
        SetPixelShader(CompileShader(ps_5_0, PixelShader()));

        SetRasterizerState(WireframeRasterizerState);
    }
}