struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
    uint viewport : SV_ViewportArrayIndex;
};

cbuffer Viewport
{
    uint viewportIndex;
};

[maxvertexcount(3)]
void main(
	triangle float4 inputPosition[3] : SV_POSITION,
    triangle float2 inputTextureCoordinate[3] : TEXCOORD,
    triangle float3 inputNormal[3] : NORMAL,
	inout TriangleStream<GSOutput> output
)
{
    for (uint i = 0; i < 3; i++)
    {
        GSOutput element;
        element.pos = inputPosition[i];
        element.textureCoordinate = inputTextureCoordinate[i];
        element.normal = inputNormal[i];
        element.viewport = viewportIndex;
        output.Append(element);
    }
}