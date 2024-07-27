#pragma once

#include "Model.h"

#include <iostream>
#include <map>


struct BoneInfo {
    glm::mat4 offsetMatrix;
    glm::mat4 finalTransformation = glm::mat4(1.0f);
    int boneKey;
    std::string nodeName;
    //std::vector<BoneInfo*> childBones;
    std::vector<std::string> childBones;
};


struct PositionKeyData {

    PositionKeyData()
    : hasPositionData(false)
    {}

    glm::vec3 position;
    bool hasPositionData;
};

struct RotationKeyData {

    RotationKeyData()
        : hasRotationData(false)
    {}

    glm::quat rotation;
    bool hasRotationData;
};

struct ScaleKeyData {

    ScaleKeyData()
        : hasScaleData(false)
    {}

    glm::vec3 scale;
    bool hasScaleData;
};



struct Keyframe {
    float time;
    
    PositionKeyData positionData;
    RotationKeyData rotationData;
    ScaleKeyData scaleData;
};

struct AnimationChannel {
    std::string nodeName;
    std::vector<AnimationChannel*> childrenChannels;
    std::vector<Keyframe> keyframes;
};

struct AnimationData {
    std::vector<AnimationChannel> channels;
    std::string name;
    float duration;
    float ticksPerSecond;
};



class SkinnedModel
{
public:
    void UpdateAnimation(float timeInSeconds);

    void ReadNodeAnimation(float animationTime, glm::mat4 parentTransform, BoneInfo* bone);

    void PrintNodeChildren(BoneInfo* childNode, int depthw);

    const AnimationChannel* FindNodeAnimation(const std::string& nodeName);

    void AddAnimation(const AnimationData& animData) {
        animations.push_back(animData);
    }

private:


    glm::mat4 AssimpMatToGlmMat(const aiMatrix4x4& from) {
        glm::mat4 to;
        to[0][0] = from.a1; to[0][1] = from.b1; to[0][2] = from.c1; to[0][3] = from.d1;
        to[1][0] = from.a2; to[1][1] = from.b2; to[1][2] = from.c2; to[1][3] = from.d2;
        to[2][0] = from.a3; to[2][1] = from.b3; to[2][2] = from.c3; to[2][3] = from.d3;
        to[3][0] = from.a4; to[3][1] = from.b4; to[3][2] = from.c4; to[3][3] = from.d4;
        return to;
    }


    glm::vec3 InterpolatePosition(float animationTime, const AnimationChannel& channel);

    glm::quat InterpolateRotation(float animationTime, const AnimationChannel& channel);

    glm::vec3 InterpolateScale(float animationTime, const AnimationChannel& channel);


    template<typename T>
    unsigned int FindKeyFrame(float animationTime, const std::vector<T>& keyframes)
    {

        if (keyframes.empty()) {
            return 0; // or handle differently based on your logic
        }

        for (unsigned int i = 0; i < keyframes.size() - 1; i++) {
            if (animationTime < keyframes[i + 1].mTime) {
                return i;
            }
        }
        return keyframes.size() - 1;
    }

    glm::mat4 ConstructTransformMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale);

public:
    std::map<std::string, BoneInfo> boneInfoMap;

    std::vector<SkinnedVertex> vertices;
    std::vector<unsigned int> indices;


    ModelTextureMaps textures;

    std::string modelPath;

    bool hasSpecular;
    bool hasDiffuse;
    

    BoneInfo* rootBone;


    //aiNode* rootBoneNode;

    std::vector<AnimationData> animations;

};