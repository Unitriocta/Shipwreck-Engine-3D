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


    glm::mat4 nodeTransformation = glm::mat4(1.0f);  // Default transformation



    




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