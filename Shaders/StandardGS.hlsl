struct GSOutput
{
    float4 pos : SV_POSITION;
    uint viewport : SV_ViewportArrayIndex;
};

cbuffer Viewport
{
    uint viewportIndex;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    for (uint i = 0; i < 3; i++)
    {
        GSOutput element;
        element.pos = input[i];
        element.viewport = viewportIndex;
        output.Append(element);
    }
}