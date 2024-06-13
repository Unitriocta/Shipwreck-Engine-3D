struct VS_In
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : UV;
};

struct VS_Out
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : UV;
};


struct PS_Out
{
    float4 color : SV_TARGET;
    //float depth : SV_Depth;
};




cbuffer CBuffer : register(b0)
{
    int hasRegular;
    int hasSpecular;
    int hasDiffuse;
    int hasNormal;
    int isLit;
};

Texture2D shaderTex : register(t0);
Texture2D specularTex : register(t1);
Texture2D diffuseTex : register(t2);
Texture2D normalTex : register(t3);
SamplerState samplerState : register(s0);


static float3 lightPos = { 0.0f, 7.0f, -5.0f };
static float3 ambient = { 0.05f, 0.05f, 0.05f };
static float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static float diffuseIntensity = 2.0f;
static float specularIntensity = 0.2f;
static float attConst = 1.0f;
static float attLin = 0.045f;
static float attQuad = 0.0075f;


PS_Out main(VS_Out input) : SV_TARGET
{
    PS_Out psout;
    //return min(float4(diffuseTex.Sample(samplerState, input.uv).rgba), float4(1.0f, 1.0f, 1.0f, 1.0f));
    if (isLit == true)
    {
        float4 textureColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
        float3 specularColor = float3(1.0f, 1.0f, 1.0f);
        float3 diffuseMapColor = float3(1.0f, 1.0f, 1.0f);
        float3 reflectDir = float3(0.0f, 0.0f, 0.0f);
    
        float3 normalMap = float3(1.0f, 1.0f, 1.0f);
    
        float diff;
    
        float3 lightDir = normalize(lightPos - input.worldPos);
        float3 viewDir = normalize(-input.worldPos);
        float distance = length(lightPos - input.worldPos);
        float attenuation = 1.0f / (attConst + attLin * distance + attQuad * distance * distance);
    
    
    
        if (hasRegular == true)
        {
            textureColor = shaderTex.Sample(samplerState, input.uv);
        }

        if (hasSpecular == true)
        {
            specularColor = specularTex.Sample(samplerState, input.uv).rgb;
        }

        if (hasDiffuse == true)
        {
            diffuseMapColor = diffuseTex.Sample(samplerState, input.uv).rgb;
        }

        if (hasNormal == true)
        {
            normalMap = normalTex.Sample(samplerState, input.uv).rgb * 2.0f - 1.0f;
            reflectDir = reflect(-lightDir, normalMap);
            diff = max(dot(normalMap, lightDir), 0.0f);
        }
        else
        {
            //reflectDir = reflect(-lightDir, input.normal);
            diff = max(dot(input.normal, lightDir), 0.0f);
        }
    
    
    
        float3 diffuse = diffuseMapColor * diffuseColor * diff * diffuseIntensity * attenuation;
    
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), specularIntensity);
        float3 specular = specularColor * spec * attenuation;
    
        float3 finalColor = textureColor.rgb * (diffuse + ambient) + specular;

        psout.color = float4(finalColor, textureColor.a);
    }
    else
    {
        if (hasDiffuse == true)
        {
            psout.color = min(float4(diffuseTex.Sample(samplerState, input.uv).rgba), float4(1.0f, 1.0f, 1.0f, 1.0f));
        }
        else
        {
            psout.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    
    //psout.depth = input.pos.z / input.pos.w;
    
    return psout;
}