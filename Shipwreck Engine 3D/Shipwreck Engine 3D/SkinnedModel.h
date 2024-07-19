#pragma once

#include "Model.h"

#include <iostream>
#include <map>


struct BoneInfo {
    glm::mat4 offsetMatrix;
    glm::mat4 finalTransformation;
    int boneKey;
};


struct Keyframe {
    float time;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

struct AnimationChannel {
    std::string nodeName;
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

    void ReadNodeAnimation(float animationTime, const aiNode* node, const glm::mat4& parentTransform);

    const AnimationChannel* FindNodeAnimation(const std::string& nodeName);

    void AddAnimation(const AnimationData& animData) {
        animations.push_back(animData);
    }

private:


    glm::vec3 InterpolatePosition(float animationTime, const AnimationChannel& channel)
    {
        if (channel.keyframes.empty()) {
            return glm::vec3(0.0f);
        }

        if (channel.keyframes.size() == 1) {
            return channel.keyframes[0].position;
        }

        size_t numKeyframes = channel.keyframes.size();
        size_t prevIndex = 0;
        size_t nextIndex = 1;

        for (size_t i = 1; i < numKeyframes; ++i) {
            if (animationTime < channel.keyframes[i].time) {
                prevIndex = i - 1;
                nextIndex = i;
                break;
            }
        }

        float deltaTime = channel.keyframes[nextIndex].time - channel.keyframes[prevIndex].time;
        float factor = (animationTime - channel.keyframes[prevIndex].time) / deltaTime;

        return glm::mix(channel.keyframes[prevIndex].position, channel.keyframes[nextIndex].position, factor);
    }

    glm::quat InterpolateRotation(float animationTime, const AnimationChannel& channel)
    {
        if (channel.keyframes.empty()) {
            return glm::quat();
        }

        if (channel.keyframes.size() == 1) {
            return channel.keyframes[0].rotation;
        }

        size_t numKeyframes = channel.keyframes.size();
        size_t prevIndex = 0;
        size_t nextIndex = 1;

        for (size_t i = 1; i < numKeyframes; ++i) {
            if (animationTime < channel.keyframes[i].time) {
                prevIndex = i - 1;
                nextIndex = i;
                break;
            }
        }

        float deltaTime = channel.keyframes[nextIndex].time - channel.keyframes[prevIndex].time;
        float factor = (animationTime - channel.keyframes[prevIndex].time) / deltaTime;

        return glm::slerp(channel.keyframes[prevIndex].rotation, channel.keyframes[nextIndex].rotation, factor);
    }

    glm::vec3 InterpolateScale(float animationTime, const AnimationChannel& channel)
    {
        if (channel.keyframes.empty()) {
            return glm::vec3(1.0f);
        }

        if (channel.keyframes.size() == 1) {
            return channel.keyframes[0].scale;
        }

        size_t numKeyframes = channel.keyframes.size();
        size_t prevIndex = 0;
        size_t nextIndex = 1;

        for (size_t i = 1; i < numKeyframes; ++i) {
            if (animationTime < channel.keyframes[i].time) {
                prevIndex = i - 1;
                nextIndex = i;
                break;
            }
        }

        float deltaTime = channel.keyframes[nextIndex].time - channel.keyframes[prevIndex].time;
        float factor = (animationTime - channel.keyframes[prevIndex].time) / deltaTime;

        return glm::mix(channel.keyframes[prevIndex].scale, channel.keyframes[nextIndex].scale, factor);
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
    std::vector<aiBone*> bones;

    std::vector<SkinnedVertex> vertices;
    std::vector<unsigned int> indices;


    ModelTextureMaps textures;

    std::string modelPath;

    bool hasSpecular;
    bool hasDiffuse;
    

    aiNode* rootBoneNode;

    std::vector<AnimationData> animations;

};