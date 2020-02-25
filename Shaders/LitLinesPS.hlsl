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
    return sqrt(1 - pow(dot(direction, lightVector), 2));
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
    float3 reflectedLight = incidentVector - 2 * dotReplacement(direction, incidentVector) * direction;
    
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
    float4 hairBaseColor = { 1, 0, 1, 1 };
    //float specularPower = 1;
    
    //float4 ambientValue = directionalLight.ambient;
    
    //float4 specularValue = directionalLight.specular * hairBaseColor;
    //specularValue *= scatterSpecular((float3) input.position, input.direction, 1);
    
    //float4 diffuseValue = hairBaseColor;
    //diffuseValue *= directionalLight.diffuse;
    //diffuseValue *= scatterDiffuse(input.direction);
    //return ambientValue + diffuseValue + specularValue;
    return hairBaseColor;
}