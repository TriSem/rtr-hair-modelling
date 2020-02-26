struct PSInput
{
    float4 position : SV_POSITION;
    float3 direction : NORMAL;
    uint viewport : SV_ViewportArrayIndex;
};

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

float dotReplacement(float3 direction, float3 lightVector)
{
    float dotProduct = dot(direction, lightVector);
    return sqrt(1 - dotProduct * dotProduct);
}

float scatterDiffuse(float3 hairDirection)
{
    float cosine = dotReplacement(hairDirection, -directionalLight.direction);
    return max(cosine, 0);
}

float scatterSpecular(float3 surfacePoint, float3 direction, float specularPower)
{
    float3 viewVector = viewPoint - surfacePoint;
    float3 incidentVector = directionalLight.direction;
    viewVector = normalize(viewVector);
    float3 reflectedLight = incidentVector - direction * dotReplacement(direction, incidentVector);
    
    float result = 0;
    if (dotReplacement(direction, -directionalLight.direction) > 0)
    {
        result = max(dot(viewVector, reflectedLight), 0);
        if (specularPower < 1.0f)
            specularPower = 1.0f;
        result = pow(result, specularPower);
    }
    
    return result;
}

float4 main(PSInput input) : SV_TARGET
{
    float4 hairBaseColor = { 1 , 0, 1, 1 };
    float4 hairSpecularColor = { 0.3f, 0.3f, 0.3f, 1 };
    float4 hairAmbientColor = { 1, 0, 1, 1 };
    float specularPower = 2;
    
    float4 ambientValue = directionalLight.ambient * hairAmbientColor;
    
    float4 specularValue = directionalLight.specular * hairSpecularColor;
    specularValue *= scatterSpecular(input.position.xyz, input.direction, specularPower);
    
    float4 diffuseValue = hairBaseColor * directionalLight.diffuse;
    diffuseValue *= scatterDiffuse(input.direction);
    return ambientValue + diffuseValue + specularValue;
}