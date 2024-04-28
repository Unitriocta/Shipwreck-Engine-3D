struct VSOut {
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer CBuffer {
	matrix transform;
	matrix camera;
};

VSOut main(float3 pos : POSITION, float4 color : COLOR)
{
	VSOut vsout;
    vsout.pos = mul(float4(pos, 1.0f), transform);
	vsout.pos = mul(vsout.pos, camera);

	vsout.color = color;
	return vsout;
}