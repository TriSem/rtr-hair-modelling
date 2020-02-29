Texture2D hairTexture : register(t0);

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
    hairVertex.position = mul(mv, float4(vertex.position, 1)).xyz;
    hairVertex.normal = mul(mv, float4(vertex.normal, 0)).xyz;
    hairVertex.tangent = mul(mv, float4(vertex.tangent, 0)).xyz;
    hairVertex.bitangent = mul(mv, float4(vertex.bitangent, 0)).xyz;
    
    uint texWidth, texHeight;
    hairTexture.GetDimensions(texWidth, texHeight);
    uint2 pos = uint2(vertex.textureCoordinate.x * texWidth, vertex.textureCoordinate.y * texHeight);
    
    float3 rgb = hairTexture[ pos ];
    HairProperties properties = { rgb.r, rgb.g, rgb.b };
    
    VSOutput output;
    output.hairVertex = hairVertex;
    output.hairProperties = properties;
    output.projectionMatrix = projection;
    
    return output;
}