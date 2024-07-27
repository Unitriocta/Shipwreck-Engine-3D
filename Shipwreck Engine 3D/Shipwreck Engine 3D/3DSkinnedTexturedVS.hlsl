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
    
    //float4 skinnedPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //float3 skinnedNormal = float3(0.0f, 0.0f, 0.0f);
    
    //for (int i = 0; i < 4; ++i)
    //{
    //  if (input.weights[i] > 0.0f)
    //  {
    //      float4 localPos = float4(input.pos, 1.0f);
    //      skinnedPos += mul(localPos, boneTransforms[input.boneIDs[i]]) * input.weights[i];
    //        //skinnedPos += boneTransforms[i][input.boneIDs[i]] * localPos * input.weights[i];
    //        
    //        skinnedNormal += mul((float3x3) boneTransforms[input.boneIDs[i]], input.normal) * input.weights[i];
    //    }
    //}
    
    //skinnedPos.w = 1.0f;
    
    vsout.pos = mul(float4(input.pos, 1.0f), transform);
    //vsout.pos = mul(skinnedPos, transform);
    vsout.pos = mul(vsout.pos, camera);

    //vsout.normal = normalize(skinnedNormal);
    vsout.normal = normalize(input.normal);

    vsout.color = input.color;

    vsout.worldPos = input.pos;
    
    vsout.uv = input.uv;

    return vsout;
}





/*struct VS_In
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

// Define unique colors for each bone
float4 GetBoneColor(int boneID)
{
    switch (boneID)
    {
        case 0:
            return float4(1.0f, 0.0f, 0.0f, 1.0f); // Red
        case 1:
            return float4(0.0f, 1.0f, 0.0f, 1.0f); // Green
        case 2:
            return float4(0.0f, 0.0f, 1.0f, 1.0f); // Blue
        case 3:
            return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
        case 4:
            return float4(1.0f, 0.5f, 0.0f, 1.0f); // Orange
        case 5:
            return float4(0.5f, 0.0f, 0.5f, 1.0f); // Purple
        case 6:
            return float4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan
        case 7:
            return float4(1.0f, 0.0f, 1.0f, 1.0f); // Magenta
        case 8:
            return float4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
        case 9:
            return float4(0.75f, 0.75f, 0.75f, 1.0f); // Light Gray
        case 10:
            return float4(0.0f, 0.5f, 0.0f, 1.0f); // Dark Green
        case 11:
            return float4(0.0f, 0.0f, 0.5f, 1.0f); // Dark Blue
        case 12:
            return float4(0.5f, 0.0f, 0.0f, 1.0f); // Dark Red
        case 13:
            return float4(0.0f, 0.5f, 0.5f, 1.0f); // Teal
        case 14:
            return float4(0.5f, 0.5f, 0.0f, 1.0f); // Olive
        case 15:
            return float4(0.5f, 0.25f, 0.0f, 1.0f); // Brown
        case 16:
            return float4(0.0f, 0.0f, 0.0f, 1.0f); // Black
        case 17:
            return float4(1.0f, 1.0f, 1.0f, 1.0f); // White
        case 18:
            return float4(0.5f, 0.0f, 0.5f, 1.0f); // Deep Purple
        case 19:
            return float4(0.75f, 0.5f, 0.5f, 1.0f); // Rosy Brown
        // Add more colors if needed
        default:
            return float4(1.0f, 1.0f, 1.0f, 1.0f); // White for unknown boneID
    }
}

VS_Out main(VS_In input)
{
    VS_Out vsout;
    
    
    vsout.pos = mul(float4(input.pos, 1.0f), transform);
    vsout.pos = mul(vsout.pos, camera);

    vsout.normal = normalize(input.normal);
    
    float maxWeight = 0.0f;
    int maxBoneID = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (input.weights[i] > maxWeight)
        {
            maxWeight = input.weights[i];
            maxBoneID = input.boneIDs[i];
        }
    }
    if (maxWeight > 0.96f)
    {
        vsout.color = GetBoneColor(maxBoneID);
    }
    else
    {
        vsout.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    vsout.worldPos = input.pos;
    vsout.uv = input.uv;

    return vsout;
}*/