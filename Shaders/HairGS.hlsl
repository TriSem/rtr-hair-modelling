struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    uint viewport : SV_ViewportArrayIndex;
};

struct HairProperties
{
    int length;
    int tangent;
    int bitangent;
};

cbuffer Viewport
{
    uint viewportIndex;
};

void interpolateBarycentric(float3 corners[3], out float3 result[7])
{
    result[0] = (corners[0] + corners[1] + corners[2]) / 3;
    result[1] = (corners[0] * 0.67f) + (corners[1] * 0.165f) + (corners[2] * 0.165f);
    result[2] = (corners[0] * 0.165f) + (corners[1] * 0.66f) + (corners[2] * 0.165f);
    result[3] = (corners[0] * 0.17f) + (corners[1] * 0.17f) + (corners[2] * 0.66f);
    result[4] = (corners[0] * 0.415f) + (corners[1] * 0.415f) + (corners[2] * 0.66f);
    result[5] = (corners[0] * 0.17f) + (corners[1] * 0.415f) + (corners[2] * 0.415f);
    result[6] = (corners[0] * 0.415f) + (corners[1] * 0.17f) + (corners[2] * 0.415f);
}

[maxvertexcount(30)]
void main(
	triangle float4 inputPosition[3] : SV_POSITION,
    triangle float2 inputTextureCoordinate[3] : TEXCOORD,
    triangle float3 inputNormal[3] : NORMAL,
    triangle float3 inputTangent[3] : TANGENT,
    triangle float3 inputBitangent[3] : BINORMAL,
    triangle HairProperties hairProperties[3],
	out LineStream<GSOutput> output
)
{
    float hairLengthAverage = hairProperties[0].length + hairProperties[1].length + hairProperties[2].length;
    hairLengthAverage /= 3;
    
    if (hairLengthAverage > 0.01f)
    {
        float3 hairPositions[7];
        float3 hairNormals[7];
        float3 hairTangents[7];
        float3 hairBitangents[7];
        float3x3 tangentSpaces[7];
        
        float3 positions[3] = { (float3) inputPosition[0], (float3) inputPosition[1], (float3) inputPosition[2] };
        
        interpolateBarycentric(positions, hairPositions);
        interpolateBarycentric(inputNormal, hairNormals);
        interpolateBarycentric(inputTangent, hairTangents);
        interpolateBarycentric(inputBitangent, hairBitangents);
        
        for (int i = 0; i < 7; i++)
        {
            float3x3 tangentSpace =
            {
                hairNormals[i], hairTangents[i], hairBitangents[i]
            };
            tangentSpaces[i] = tangentSpace;
        }
    }
    
    //for (uint i = 0; i < 3; i++)
    //{
    //    GSOutput element;
    //    element.pos = inputPosition[i];
    //    element.textureCoordinate = inputTextureCoordinate[i];
    //    element.normal = inputNormal[i];
    //    element.viewport = viewportIndex;
    //    output.Append(element);
    //}
}