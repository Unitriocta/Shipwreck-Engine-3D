struct VS_In
{
	float3 pos: POSITION;
	float3 normal: NORMAL;
    float4 color : COLOR;
    float2 uv : UV;
};

struct VS_Out
{
	float4 pos: SV_POSITION;
	float3 worldPos: POSITION;
	float3 normal: NORMAL;
    float4 color : COLOR;
    float2 uv : UV;
};


cbuffer CBuffer : register(b0)
{
	matrix transform;
	matrix camera;
};


VS_Out main(VS_In input)
{
	VS_Out vsout;
	vsout.pos = mul(float4(input.pos, 1.0f), transform);
	vsout.pos = mul(vsout.pos, camera);

	vsout.normal = mul(input.normal, (float3x3)transform);

	vsout.normal = normalize(vsout.normal);

	vsout.color = input.color;

	vsout.worldPos = input.pos;
	
    vsout.uv = input.uv;

	return vsout;
}