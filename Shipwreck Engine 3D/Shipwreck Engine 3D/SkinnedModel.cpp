#include "SkinnedModel.h"
#include "StartEngine.h"
#include "FileManager.h"
using namespace FileManager;



std::string THEdata = "";

void SkinnedModel::PrintNodeChildren(BoneInfo* childNode, int depth) {
    




    THEdata += std::string(depth * 2, ' ');
    THEdata += childNode->nodeName;
    THEdata += "\n";


    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(childNode->finalTransformation, scale, rotation, position, skew, perspective);

    glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));

    THEdata += "\nBone Position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ", ";
    THEdata += "\nBone Rotation: " + std::to_string(eulerAngles.x) + ", " + std::to_string(eulerAngles.y) + ", " + std::to_string(eulerAngles.z) + ", ";
    THEdata += "\nBone Scale: " + std::to_string(scale.x) + ", " + std::to_string(scale.y) + ", " + std::to_string(scale.z) + ", ";



    for (unsigned int i = 0; i < childNode->childBones.size(); ++i) {
        PrintNodeChildren(&boneInfoMap[childNode->childBones[i]], depth + 1);
    }
}


void SkinnedModel::UpdateAnimation(float time) {
    if (!animations.empty()) {

        float animationTime = std::fmod((time / 1000.0f) * animations[0].ticksPerSecond, animations[0].duration);

        std::string fileSavePath = "D:/F3.txt";
        THEdata = "";
        THEdata += std::to_string(time);
        THEdata += std::to_string(animationTime);

        ReadNodeAnimation(animationTime, glm::mat4(1.0f), &boneInfoMap[rootBone->nodeName]);  // Start the recursive reading from the root bone node

        if (ReadFromFile(fileSavePath) == "") {
            for (auto& curBone : boneInfoMap) {
                glm::vec3 position;
                glm::quat rotation;
                glm::vec3 scale;

                glm::vec3 skew;
                glm::vec4 perspective;

                glm::decompose(curBone.second.finalTransformation, scale, rotation, position, skew, perspective);

                glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));

                THEdata += "\nBone Name: " + curBone.first + ",";
                THEdata += "\nBone Node Name: " + curBone.second.nodeName + ",";
                THEdata += "\nBone Position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ", ";
                THEdata += "\nBone Rotation: " + std::to_string(eulerAngles.x) + ", " + std::to_string(eulerAngles.y) + ", " + std::to_string(eulerAngles.z) + ", ";
                THEdata += "\nBone Scale: " + std::to_string(scale.x) + ", " + std::to_string(scale.y) + ", " + std::to_string(scale.z) + ", ";
            }
            THEdata += "\nROOTBONENAME: " + rootBone->nodeName + "\n\n";
            PrintNodeChildren(&boneInfoMap[rootBone->nodeName], 0);
            WriteDataToFile(fileSavePath, THEdata);
        }
    }
}


void SkinnedModel::ReadNodeAnimation(float animationTime, glm::mat4 parentTransform, BoneInfo* bone) {
    const AnimationChannel* channel = FindNodeAnimation(bone->nodeName);

        
    if (channel) {

        glm::mat4 nodeTransformation = glm::mat4(1.0f);








        glm::vec3 position = InterpolatePosition(animationTime, *channel);
        glm::quat rotation = InterpolateRotation(animationTime, *channel);
        glm::vec3 scale = InterpolateScale(animationTime, *channel);

        /*glm::vec3 eulerAngles = glm::eulerAngles(rotation);
        eulerAngles.z = -eulerAngles.z;
        rotation = glm::quat(eulerAngles);*/


        nodeTransformation = ConstructTransformMatrix(position, rotation, scale);


        glm::mat4 globalTransformation = parentTransform * nodeTransformation;

        bone->finalTransformation = globalTransformation * bone->offsetMatrix;


        THEdata += "\n";
        glm::vec3 lposition;
        glm::quat lrotation;
        glm::vec3 lscale;

        glm::vec3 skew;
        glm::vec4 perspective;

        glm::decompose(bone->finalTransformation, lscale, lrotation, lposition, skew, perspective);

        glm::vec3 leulerAngles = glm::degrees(glm::eulerAngles(lrotation));

        THEdata += "\Node Position: " + std::to_string(lposition.x) + ", " + std::to_string(lposition.y) + ", " + std::to_string(lposition.z) + ", ";
        THEdata += "\Node Rotation: " + std::to_string(leulerAngles.x) + ", " + std::to_string(leulerAngles.y) + ", " + std::to_string(leulerAngles.z) + ", ";
        THEdata += "\nNode Scale: " + std::to_string(lscale.x) + ", " + std::to_string(lscale.y) + ", " + std::to_string(lscale.z) + ", ";


        for (std::string childBone : bone->childBones) {
            ReadNodeAnimation(animationTime, globalTransformation, &boneInfoMap[childBone]);
        }
    }
}

const AnimationChannel* SkinnedModel::FindNodeAnimation(const std::string& nodeName) {

    //for (const auto& animation : animations) {
        for (const auto& channel : animations[0].channels) {
            if (channel.nodeName == nodeName) {
                return &channel;
            }
        }
    //}
    return nullptr;
}






glm::vec3 SkinnedModel::InterpolatePosition(float animationTime, const AnimationChannel& channel)
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

    // Handle boundary conditions
    if (animationTime <= validKeyframes.front().time) {
        return validKeyframes.front().positionData.position;
    }
    if (animationTime >= validKeyframes.back().time) {
        return validKeyframes.back().positionData.position;
    }

    float deltaTime = validKeyframes[nextIndex].time - validKeyframes[prevIndex].time;
    float factor = (animationTime - validKeyframes[prevIndex].time) / deltaTime;

    return glm::mix(validKeyframes[prevIndex].positionData.position, validKeyframes[nextIndex].positionData.position, factor) * glm::vec3(0.0f, 1.0f, 0.0f);
}



glm::quat SkinnedModel::InterpolateRotation(float animationTime, const AnimationChannel& channel)
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

    // Find the previous and next keyframe indices
    for (size_t i = 1; i < numKeyframes; ++i) {
        if (animationTime < validKeyframes[i].time) {
            prevIndex = i - 1;
            nextIndex = i;
            break;
        }
        // If the animationTime is exactly at the last keyframe time
        if (i == numKeyframes - 1) {
            prevIndex = i - 1;
            nextIndex = i;
        }
    }

    // Handle boundary conditions
    if (animationTime <= validKeyframes.front().time) {
        return validKeyframes.front().rotationData.rotation;
    }
    if (animationTime >= validKeyframes.back().time) {
        return validKeyframes.back().rotationData.rotation;
    }

    float deltaTime = validKeyframes[nextIndex].time - validKeyframes[prevIndex].time;
    float factor = (animationTime - validKeyframes[prevIndex].time) / deltaTime;

    return glm::normalize(glm::slerp(validKeyframes[prevIndex].rotationData.rotation, validKeyframes[nextIndex].rotationData.rotation, factor));
}



glm::vec3 SkinnedModel::InterpolateScale(float animationTime, const AnimationChannel& channel)
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

    // Handle boundary conditions
    if (animationTime <= validKeyframes.front().time) {
        return validKeyframes.front().scaleData.scale;
    }
    if (animationTime >= validKeyframes.back().time) {
        return validKeyframes.back().scaleData.scale;
    }

    float deltaTime = validKeyframes[nextIndex].time - validKeyframes[prevIndex].time;
    float factor = (animationTime - validKeyframes[prevIndex].time) / deltaTime;

    return glm::mix(validKeyframes[prevIndex].scaleData.scale, validKeyframes[nextIndex].scaleData.scale, factor)/* * glm::vec3(1.0f, 1.0f, 1.0f)*/;
}




glm::mat4 SkinnedModel::ConstructTransformMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    //glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    return transformMatrix;
}