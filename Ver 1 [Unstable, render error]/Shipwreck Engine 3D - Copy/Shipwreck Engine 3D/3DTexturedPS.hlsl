struct VS_In
{
	float3 pos: POSITION;
	float3 normal: NORMAL;
	float4 color: COLOR;
};

struct VS_Out
{
	float4 pos: SV_POSITION;
	float3 worldPos: POSITION;
	float3 normal: NORMAL;
	float4 color: COLOR;
};

static float3 lightPos = { 0.0f, 7.0f, -5.0f };

static float3 ambient = { 0.05f, 0.05f, 0.05f};
static float3 diffuseColor = { 1.0f, 1.0f, 1.0f};
static float diffuseIntensity = 2.0f;

static float3 sampleCol = {0.5f,0.5f,0.5f};


static float attConst = 1.0f;
static float attLin = 0.045f;
static float attQuad = 0.0075f;



float4 main(VS_Out input) : SV_TARGET
{
	float3 lightOffset = lightPos - input.worldPos;
	float distance = length(lightOffset);
	float3 direction = lightOffset / distance;

	float att = 1.0f / (attConst + attLin * distance + attQuad * (distance * distance));

	float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(direction, input.normal) );


	////return float4(0.05f,0.5f,0.5f,1);
	//return min(float4(loadedTexture.Sample(samplerState, input.uv).rgba), input.color);

	return float4(sampleCol * saturate(diffuse + ambient), 1.0f);
}