struct VS_In
{
	float2 pos: POSITION;
	float2 uv: UV;
};

struct VS_Out
{
	float4 pos: SV_POSITION;
	float2 uv: UV;
};


cbuffer CBuffer {
	matrix transform;
};

VS_Out main(VS_In input)
{
	VS_Out output;
	output.pos = mul(float4(input.pos, 0.0f, 1.0f), transform);
	output.uv = input.uv;
	return output;
};