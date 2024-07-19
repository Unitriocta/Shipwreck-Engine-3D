#include "SkinnedModel.h"

#include "FileManager.h"
using namespace FileManager;

void SkinnedModel::UpdateAnimation(float time) {
    if (!animations.empty()) {
        
        float animationTime = std::fmod(time * animations[0].ticksPerSecond, animations[0].duration);

        std::string fileSavePath = "D:/DATATTOSSOKFDOKSAOF";
        std::string data;
        data += "TOtAlDurNgATion:";
        data += std::to_string(animationTime);
        data += "\nDurNATion:";
        data += std::to_string(animations[0].duration);
        data += "\nTPS:";
        data += std::to_string(animations[0].ticksPerSecond);

        WriteDataToFile(fileSavePath, data);

        ReadNodeAnimation(animationTime, rootBoneNode, glm::mat4(1.0f));
    }
}


void SkinnedModel::ReadNodeAnimation(float animationTime, const aiNode* node, const glm::mat4& parentTransform) {
    std::string nodeName(node->mName.C_Str());

    const AnimationChannel* channel = FindNodeAnimation(nodeName);
    if (channel) {
        glm::mat4 nodeTransformation = ConstructTransformMatrix(InterpolatePosition(animationTime, *channel), InterpolateRotation(animationTime, *channel), InterpolateScale(animationTime, *channel));//CalculateInterpolatedTransform(animationTime, *channel);

        glm::mat4 globalTransformation = parentTransform * nodeTransformation;

        boneInfoMap[nodeName].finalTransformation = globalTransformation * boneInfoMap[nodeName].offsetMatrix;

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ReadNodeAnimation(animationTime, node->mChildren[i], globalTransformation);
        }
    }
}

const AnimationChannel* SkinnedModel::FindNodeAnimation(const std::string& nodeName) {
    for (const auto& animation : animations) {
        for (const auto& channel : animation.channels) {
            if (channel.nodeName == nodeName) {
                return &channel;
            }
        }
    }
    return nullptr;
}