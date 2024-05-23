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
/*
UV - The coordinates on an image to wrap the pixels on a triangle/rect,
		starts from the top left, it goes right and down

		(0, 0)-----------------------------(1, 0)
		  |                                   |
		  |                                   |
		  |                                   |
		  |               Image               |
		  |                                   |
		  |                                   |
		  |                                   |
		  |                                   |
		(0, 1)-----------------------------(1, 1)
*/

Texture2D loadedTexture;
SamplerState samplerState;

float4 main(VS_Out input) : SV_TARGET
{
	return min(float4(loadedTexture.Sample(samplerState, input.uv).rgba), float4(1.0f,1.0f,1.0f,1.0f)); //min() MEANING: It returns the lower value and combines all 4 minimum values into 1 float4/Vector4
	//Ex: min(float4(0,1,0,1),float4(1,0,1,0)) returns float4(0,0,0,0)
	//Ex: min(float4(0,1,0,1),float4(1,0,1,1)) returns float4(0,0,0,1)
};