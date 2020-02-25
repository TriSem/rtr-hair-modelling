struct OutputVertex
{
    float4 position : SV_POSITION;
    float3 direction : NORMAL;
    uint viewport : SV_ViewportArrayIndex;
};

struct HairProperties
{
    float length;
    float tangent;
    float bitangent;
};

struct HairVertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
    HairProperties hairProperties : HAIR;
    float4x4 projection : MATRIX;
};

struct HairDefinition
{
    float length;
    float3 basePosition;
    float3x3 transposeTangentSpace;
    float3x3 rotation;
};

cbuffer Viewport
{
    uint viewportIndex;
};

float PI = 3.14159;

float mapToPiInterval(unorm float value)
{
    return value * PI - PI;
}

HairVertex interpolateBarycentric(HairVertex corners[3], float3 barycentricCoordinate)
{
    HairVertex hairVertex;
    float3 position;
    for (int i = 0; i < 3; i++)
    {
        position += corners[i].position * barycentricCoordinate[i];
        hairVertex.normal += corners[i].normal * barycentricCoordinate[i];
        hairVertex.tangent += corners[i].tangent * barycentricCoordinate[i];
        hairVertex.bitangent += corners[i].bitangent * barycentricCoordinate[i];
    }
    hairVertex.position = position;
    return hairVertex;
}

float3x3 createHairSegmentRotation(HairProperties properties)
{
    float tangentRotationAngle = mapToPiInterval(properties.tangent) / properties.length;
    float bitangentRotationAngle = mapToPiInterval(properties.bitangent) / properties.length;
        
    float3x3 tangentRotation =
    {
        1, 0, 0,
            0, cos(tangentRotationAngle), -sin(tangentRotationAngle),
            0, sin(tangentRotationAngle), cos(tangentRotationAngle)
    };
        
    float3x3 bitangentRotation =
    {
        cos(bitangentRotationAngle), 0, -sin(bitangentRotationAngle),
          0, 1, 0,
          sin(bitangentRotationAngle), 0, cos(bitangentRotationAngle)
    };

    return mul(bitangentRotation, tangentRotation);
}

HairProperties averageHairProperties(HairProperties properties[3])
{
    float lengthAverage = (properties[0].length + properties[1].length + properties[2].length) / 3;
    float tangentAverage = (properties[0].tangent + properties[1].tangent + properties[2].tangent) / 3;
    float bitangentAverage = (properties[0].bitangent + properties[1].bitangent + properties[2].bitangent) / 3;
    HairProperties average = { lengthAverage, tangentAverage, bitangentAverage };
    return average;
}

void createHair(HairDefinition hairDefinition, out OutputVertex vertices[6])
{
    OutputVertex hairVertices[6];
    hairVertices[0].position = float4(hairDefinition.basePosition, 1);
    hairVertices[0].direction = float3(0, 0, 1);
    hairVertices[0].viewport = viewportIndex;
            
    for (uint j = 1; j <= 5; j++)
    {
        hairVertices[j].direction = mul(hairDefinition.rotation, hairVertices[j - 1].direction);
        float3 finalPosition = (float3) hairVertices[j - 1].position;
        float3 worldSpaceDirection = mul(hairDefinition.transposeTangentSpace, hairVertices[j].direction);
        finalPosition += hairDefinition.length * worldSpaceDirection;
        hairVertices[j].position = float4(finalPosition, 1);
        hairVertices[j].viewport = viewportIndex;
    }
}

[maxvertexcount(42)]
void main(
	triangle HairVertex inputVertices[3],
	inout LineStream<OutputVertex> output
)
{
    //HairProperties hairProperties[3] =
    //{
    //    { 10, 1, 1 },
    //    { 10, 1, 1 },
    //    { 10, 1, 1 }
    //};
    HairProperties hairProperties[3] = { inputVertices[0].hairProperties, inputVertices[1].hairProperties, inputVertices[2].hairProperties };
    HairProperties hairAverages = averageHairProperties(hairProperties);
    
    if (hairAverages.length > 0.01f)
    {
        float3x3 rotation = createHairSegmentRotation(hairAverages);
        float4x4 projection = inputVertices[0].projection;
        HairVertex interpolated[7];
        interpolated[0] = interpolateBarycentric(inputVertices, float3(0.33f, 0.33f, 0.33f));
        interpolated[1] = interpolateBarycentric(inputVertices, float3(0.67f, 0.165f, 0.165f));
        interpolated[2] = interpolateBarycentric(inputVertices, float3(0.165f, 0.67f, 0.165f));
        interpolated[3] = interpolateBarycentric(inputVertices, float3(0.165f, 0.165f, 0.67f));
        interpolated[4] = interpolateBarycentric(inputVertices, float3(0.415f, 0.415f, 0.17f));
        interpolated[5] = interpolateBarycentric(inputVertices, float3(0.17f, 0.415f, 0.415f));
        interpolated[6] = interpolateBarycentric(inputVertices, float3(0.415f, 0.17f, 0.415f));
        
        OutputVertex vertexBuffer[42];
    
        //for (int i = 0; i < 6; i++)
        //{
        float3x3 tangentSpace =
        {
            interpolated[0].tangent, interpolated[0].bitangent, interpolated[0].normal
        };
            
        float3x3 transposeTangentSpace = transpose(tangentSpace);
        //    HairDefinition hairDefinition = { hairAverages.length, (float3) interpolated[i].position, transposeTangentSpace, rotation };
        //    OutputVertex vertices[6];
        //    createHair(hairDefinition, vertices);
        
        //    vertexBuffer[i * 7] = vertices[0];
        //    vertexBuffer[i * 7 + 1] = vertices[1];
        //    vertexBuffer[i * 7 + 2] = vertices[2];
        //    vertexBuffer[i * 7 + 3] = vertices[3];
        //    vertexBuffer[i * 7 + 4] = vertices[4];
        //    vertexBuffer[i * 7 + 5] = vertices[5];
        //}
    
        //for (uint j = 0; j < 42; j++)
        //{
        //    if (j > 0 && j % 6 == 0)
        //        output.RestartStrip();
        //    OutputVertex outputVertex = vertexBuffer[j];
        //    output.Append(outputVertex);
        //}
        
        float3 d0 = float3(0, 0, 1);
        float3 d1 = mul(rotation, d0);
        float3 d2 = mul(rotation, d1);
        float3 p0 = mul(transposeTangentSpace, interpolated[0].position);
        float3 p1 = p0 + hairAverages.length * mul(transposeTangentSpace, d1);
        float3 p2 = p1 + hairAverages.length * mul(transposeTangentSpace, d2);
        
        p0 = mul(transposeTangentSpace, p0);
        p1 = mul(transposeTangentSpace, p1);
        p2 = mul(transposeTangentSpace, p2);
        
        OutputVertex vertex;
        vertex.viewport = 1;
        vertex.direction = d0;
        vertex.position = float4(p0, 1);
        output.Append(vertex);
        
        vertex.direction = d1;
        vertex.position = float4(p1, 1);
        output.Append(vertex);
        
        vertex.direction = d2;
        vertex.position = float4(p2, 1);
        output.Append(vertex);
    }
}