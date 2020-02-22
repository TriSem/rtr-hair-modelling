Texture2D diffuseTexture : register(t0);
SamplerState samplerState : register(s0);

struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
};

cbuffer Lighting
{
    DirectionalLight directionalLight;
    float3 viewPoint;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

float4 calculateDirectionalLighting()
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

// WARNING: Not providing unit vectors for both parameters will produce incorrect results!
float diffuseLambertCosine(float3 lightDirection, float3 surfaceNormal)
{
    float cosine = dot(directionalLight.direction, surfaceNormal);
    return max(cosine, 0);
}

float specularLambertCosine(float3 surfacePoint, float3 surfaceNormal)
{
    float3 viewVector = viewPoint - surfacePoint;
    viewVector = normalize(viewVector);
    float3 reflectedLight = reflect(-directionalLight.direction, surfaceNormal);
    
    float result = 0;
    if(dot(directionalLight.direction, surfaceNormal) > 0)
    {
        result = max(dot(viewVector, reflectedLight), 0); // Specular power is currently assumed to be 1.
    }
    
}

float4 main(PSInput input, uint viewport : SV_ViewportArrayIndex) : SV_TARGET
{
    float4 ambientValue = directionalLight.ambient;
    float4 specularValue = directionalLight.specular;
    
    float4 diffuseValue = diffuseTexture.Sample(samplerState, input.textureCoordinate);
    diffuseValue *= directionalLight.diffuse;
    diffuseValue *= diffuseLambertCosine(directionalLight.direction, input.normal);
    return diffuseValue;
}