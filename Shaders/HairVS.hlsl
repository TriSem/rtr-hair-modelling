cbuffer mvpMatrix
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

struct HairProperties
{
    float length;
    float tangent;
    float bitangent;
};

struct VSInput
{
    float3 position : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
};

struct HairVertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BINORMAL;
};

struct VSOutput
{
    HairVertex hairVertex : HAIR_VERTEX;
    HairProperties hairProperties : HAIR_PROPERTIES;
    float4x4 projectionMatrix : MATRIX;
};

VSOutput main(VSInput vertex)
{
    float4x4 mv = mul(view, model);
    
    HairVertex hairVertex;
    hairVertex.position = mul(mv, float4(vertex.position, 1));
    hairVertex.normal = (float3) mul(mv, float4(vertex.normal, 0));
    hairVertex.tangent = (float3) mul(mv, float4(vertex.tangent, 0));
    hairVertex.bitangent = (float3) mul(mv, float4(vertex.bitangent, 0));
    
    HairProperties properties = { 0.05, 0, 0 };
    
    VSOutput output;
    output.hairVertex = hairVertex;
    output.hairProperties = properties;
    output.projectionMatrix = projection;
    
    return output;
}