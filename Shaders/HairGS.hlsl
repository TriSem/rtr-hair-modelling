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
};

struct GSInput
{
    HairVertex hairVertex : HAIR_VERTEX;
    HairProperties hairProperties : HAIR_PROPERTIES;
    float4x4 projectionMatrix : MATRIX;
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
    HairVertex hairVertex = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
    float3 position = { 0, 0, 0 };
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
    float tangentRotationAngle = properties.tangent;
    float bitangentRotationAngle = properties.bitangent;
        
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

[maxvertexcount(42)]
void main(
	triangle GSInput input[3],
	inout LineStream<OutputVertex> output
)
{
    HairProperties hairProperties[3] = { input[0].hairProperties, input[1].hairProperties, input[2].hairProperties };
    HairProperties hairAverages = averageHairProperties(hairProperties);
    
    if (hairAverages.length > 0.01f)
    {
        HairVertex inputVertices[3] =
        {
            input[0].hairVertex, input[1].hairVertex, input[2].hairVertex
        };
        
        HairVertex interpolated[7];
        interpolated[0] = interpolateBarycentric(inputVertices, float3(0.33f, 0.33f, 0.33f));
        interpolated[1] = interpolateBarycentric(inputVertices, float3(0.67f, 0.165f, 0.165f));
        interpolated[2] = interpolateBarycentric(inputVertices, float3(0.165f, 0.67f, 0.165f));
        interpolated[3] = interpolateBarycentric(inputVertices, float3(0.165f, 0.165f, 0.67f));
        interpolated[4] = interpolateBarycentric(inputVertices, float3(0.415f, 0.415f, 0.17f));
        interpolated[5] = interpolateBarycentric(inputVertices, float3(0.17f, 0.415f, 0.415f));
        interpolated[6] = interpolateBarycentric(inputVertices, float3(0.415f, 0.17f, 0.415f));
        
        float3x3 rotation = createHairSegmentRotation(hairAverages);
    
        for (int i = 0; i < 6; i++)
        {
            float4x4 tangentSpace =
            {
                interpolated[i].tangent, 0,
                interpolated[i].bitangent, 0,
                interpolated[i].normal, 0,
                0, 0, 0, 1
            };
            
            float4x4 transposeTangentSpace = transpose(tangentSpace);
            
            float4 direction = { 0, 0, 1, 0 };
            float4 position = { interpolated[i].position, 1 };
            float4 worldDirection = mul(transposeTangentSpace, direction);
            
            OutputVertex vertex;
            vertex.viewport = 1;
            vertex.position = mul(input[0].projectionMatrix, position);
            vertex.direction = mul(input[0].projectionMatrix, worldDirection).xyz;
            output.Append(vertex);
            
            float length = hairAverages.length / 5;
            
            for (uint j = 1; j <= 5; j++)
            {
                direction.xyz += normalize(mul(rotation, direction.xyz));
                worldDirection = mul(transposeTangentSpace, direction);
                position.xyz += length * worldDirection.xyz;
                
                vertex.position = mul(input[0].projectionMatrix, position);
                vertex.direction = mul(input[0].projectionMatrix, worldDirection).xyz;
                vertex.direction = normalize(vertex.direction);
                output.Append(vertex);
            }
        }
    }
}