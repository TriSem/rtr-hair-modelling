float4 main(float4 position : SV_POSITION, float3 normal : NORMAL) : SV_TARGET
{
    return float4(normal, 1.0f);
}