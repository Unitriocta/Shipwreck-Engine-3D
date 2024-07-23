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


    glm::vec3 InterpolatePosition(float animationTime, const AnimationChannel& channel)
    {
        if (channel.keyframes.empty()) {
            return glm::vec3(0.0f);
        }

        std::vector<Keyframe> validKeyframes;
        for (const auto& keyframe : channel.keyframes) {
            if (keyframe.positionData.hasPositionData) {
                validKeyframes.push_back(keyframe);
            }
        }

        if (validKeyframes.empty()) {
            return glm::vec3(0.0f);
        }

        if (validKeyframes.size() == 1) {
            return validKeyframes[0].positionData.position;
        }

        size_t numKeyframes = validKeyframes.size();
        size_t prevIndex = 0;
        size_t nextIndex = 1;

        for (size_t i = 1; i < numKeyframes; ++i) {
            if (animationTime < validKeyframes[i].time) {
                prevIndex = i - 1;
                nextIndex = i;
                break;
            }
        }

        float deltaTime = validKeyframes[nextIndex].time - validKeyframes[prevIndex].time;
        float factor = (animationTime - validKeyframes[prevIndex].time) / deltaTime;

        return glm::mix(validKeyframes[prevIndex].positionData.position, validKeyframes[nextIndex].positionData.position, factor);
    }

    glm::quat InterpolateRotation(float animationTime, const AnimationChannel& channel)
    {
        if (channel.keyframes.empty()) {
            return glm::quat();
        }

        std::vector<Keyframe> validKeyframes;
        for (const auto& keyframe : channel.keyframes) {
            if (keyframe.rotationData.hasRotationData) {
                validKeyframes.push_back(keyframe);
            }
        }

        if (validKeyframes.empty()) {
            return glm::quat();
        }

        if (validKeyframes.size() == 1) {
            return validKeyframes[0].rotationData.rotation;
        }

        size_t numKeyframes = validKeyframes.size();
        size_t prevIndex = 0;
        size_t nextIndex = 1;

        for (size_t i = 1; i < numKeyframes; ++i) {
            if (animationTime < validKeyframes[i].time) {
                prevIndex = i - 1;
                nextIndex = i;
                break;
            }
        }

        float deltaTime = validKeyframes[nextIndex].time - validKeyframes[prevIndex].time;
        float factor = (animationTime - validKeyframes[prevIndex].time) / deltaTime;

        return glm::slerp(validKeyframes[prevIndex].rotationData.rotation, validKeyframes[nextIndex].rotationData.rotation, factor);
    }

    glm::vec3 InterpolateScale(float animationTime, const AnimationChannel& channel)
    {
        if (channel.keyframes.empty()) {
            return glm::vec3(1.0f);
        }

        std::vector<Keyframe> validKeyframes;
        for (const auto& keyframe : channel.keyframes) {
            if (keyframe.scaleData.hasScaleData) {
                validKeyframes.push_back(keyframe);
            }
        }

        if (validKeyframes.empty()) {
            return glm::vec3(1.0f);
        }

        if (validKeyframes.size() == 1) {
            return validKeyframes[0].scaleData.scale;
        }

        size_t numKeyframes = validKeyframes.size();
        size_t prevIndex = 0;
        size_t nextIndex = 1;

        for (size_t i = 1; i < numKeyframes; ++i) {
            if (animationTime < validKeyframes[i].time) {
                prevIndex = i - 1;
                nextIndex = i;
                break;
            }
        }

        float deltaTime = validKeyframes[nextIndex].time - validKeyframes[prevIndex].time;
        float factor = (animationTime - validKeyframes[prevIndex].time) / deltaTime;

        return glm::mix(validKeyframes[prevIndex].scaleData.scale, validKeyframes[nextIndex].scaleData.scale, factor);
    }


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

    glm::mat4 ConstructTransformMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

        glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        return transformMatrix;
    }

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