struct VS_In
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : UV;
    int4 boneIDs : BLENDINDICES0; //BLENDINDICES;
    float4 weights : BLENDWEIGHT0; //BLENDWEIGHT;
};

struct VS_Out
{
    float4 pos : SV_POSITION;
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : UV;
};


cbuffer CBuffer : register(b0)
{
    matrix transform;
    matrix camera;
};

cbuffer BoneBuffer : register(b1)
{
    matrix boneTransforms[100]; //Maybe upper this number
};


VS_Out main(VS_In input)
{
    VS_Out vsout;
    
    float4 skinnedPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 skinnedNormal = float3(0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < 4; ++i)
    {
        if (input.weights[i] > 0.0f)
        {
            float4 localPos = float4(input.pos, 1.0f);
            skinnedPos += mul(localPos, boneTransforms[input.boneIDs[i]]) * input.weights[i];
            //skinnedPos += boneTransforms[i][input.boneIDs[i]] * localPos * input.weights[i];
            
            skinnedNormal += mul((float3x3) boneTransforms[input.boneIDs[i]], input.normal) * input.weights[i];
        }
    }
    
    skinnedPos.w = 1.0f;
    
    // Apply transformations
    //vsout.pos = mul(float4(input.pos, 1.0f), transform);
    vsout.pos = mul(skinnedPos, transform);
    vsout.pos = mul(vsout.pos, camera);

    vsout.normal = normalize(skinnedNormal);

    vsout.color = input.color;

    vsout.worldPos = input.pos;
    
    vsout.uv = input.uv;

    return vsout;
}