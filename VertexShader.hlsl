float4 main(float3 pos : SV_POSITION ) : SV_POSITION
{
    pos *= 0.1f;
	return float4(pos, 1.0f);
}