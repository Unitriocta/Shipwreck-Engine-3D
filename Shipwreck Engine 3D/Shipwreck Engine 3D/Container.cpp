#include "Container.h"

#include "StartEngine.h"
using namespace EngineInstance;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "FileManager.h"
using namespace FileManager;


Container::Container() :
	models(Models()), transform(Transform()), camera(Camera()), rigidbody(Rigidbody())
{
	rigidbody.transform = &transform;
	transform.container = this;

	startEng.containers.push_back(this);


	importer = new Assimp::Importer();
}




void Container::ImportModel(const char* filename) {

	//static Assimp::Importer importer; // moving this to Container would fix it, then it allocates an unneccessary amount of data (for future reference to optomize it)

	const aiScene* scene = importer->ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_ConvertToLeftHanded |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_FixInfacingNormals |
		aiProcess_SortByPType |
		aiProcess_FindDegenerates |
		aiProcess_PopulateArmatureData |
		aiProcess_FindInvalidData);


	if (!scene) {
		return;
	}

	if (isD3D) {
		LoadScene(this, scene, filename, startEng.D3DGfx().device);
	}
	else {
		LoadScene(this, scene, filename, nullptr);
	}
}
std::string dataToWrite;


void Container::LoadScene(Container* rootContainer, const aiScene* scene, const char* filename, ID3D11Device* device) {

	fs::path filepath = filename;
	std::string modelDirectory = filepath.parent_path().string();
	LoadHierarchy(scene->mRootNode, rootContainer, scene, modelDirectory);
}

void Container::StoreModels(Models* models, aiNode* node, const aiScene* scene, std::string& modelDir) {

	dataToWrite = "Container Name:" + name + "\n";


	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		if (!scene->mMeshes[node->mMeshes[i]]->HasBones()) {

			Model mesh;
			mesh.vertices.resize(scene->mMeshes[node->mMeshes[i]]->mNumVertices);


			for (unsigned int j = 0; j < scene->mMeshes[node->mMeshes[i]]->mNumVertices; j++)
			{
				if (scene->mMeshes[node->mMeshes[i]]->HasPositions())
				{
					mesh.vertices[j].position.x = scene->mMeshes[node->mMeshes[i]]->mVertices[j].x;
					mesh.vertices[j].position.y = scene->mMeshes[node->mMeshes[i]]->mVertices[j].y;
					mesh.vertices[j].position.z = scene->mMeshes[node->mMeshes[i]]->mVertices[j].z;
				}

				if (scene->mMeshes[node->mMeshes[i]]->HasNormals())
				{
					mesh.vertices[j].normal.x = scene->mMeshes[node->mMeshes[i]]->mNormals[j].x;
					mesh.vertices[j].normal.y = scene->mMeshes[node->mMeshes[i]]->mNormals[j].y;
					mesh.vertices[j].normal.z = scene->mMeshes[node->mMeshes[i]]->mNormals[j].z;
				}
				if (scene->mMeshes[node->mMeshes[i]]->HasTextureCoords(0)) {
					mesh.vertices[j].uv.x = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].x;
					mesh.vertices[j].uv.y = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].y;
				}
			}


			if (scene->mMeshes[node->mMeshes[i]]->mMaterialIndex >= 0) {
				const aiMaterial* material = scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];

				aiString path;
				fs::path fullPath;
				std::string fullPathStr;

				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					mesh.hasDiffuse = true;

					std::string narrowString = path.C_Str();
					fullPath = fs::path(modelDir) / narrowString;
					fullPathStr = fullPath.string();
					std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

					texLoader.LoadTextureFromFile(startEng.D3DGfx().device, fullPathStr, &mesh.textures.diffuseTex);
				}
				else {
					mesh.hasDiffuse = false;
				}


				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {

					mesh.hasSpecular = true;

					std::string narrowString = path.C_Str();
					fullPath = fs::path(modelDir) / narrowString;
					fullPathStr = fullPath.string();
					std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

					texLoader.LoadTextureFromFile(startEng.D3DGfx().device, fullPathStr, &mesh.textures.specularTex);
				}
				else {
					mesh.hasSpecular = false;
				}
			}

			for (unsigned int m = 0; m < scene->mMeshes[node->mMeshes[i]]->mNumFaces; m++)
			{
				const struct aiFace* face = &scene->mMeshes[node->mMeshes[i]]->mFaces[m];
				for (unsigned int k = 0; k < face->mNumIndices; k++)
				{
					mesh.indices.push_back(face->mIndices[k]);
				}
			}

			models->AddModel(&mesh);
		}
		else {


			//Populate parent-child node name relationships





			SkinnedModel* skinnedMesh = new SkinnedModel();


			std::unordered_set<std::string> boneNames;

			CollectBoneNames(scene, boneNames);
			aiNode* rootBone = FindRootBoneNode(scene->mRootNode, boneNames);

			if (rootBone) {

				dataToWrite += "\n\nRoot Bone Info: ";

				dataToWrite += "\nName: ";
				dataToWrite += rootBone->mName.C_Str();

				dataToWrite += "\nNum Children: ";
				dataToWrite += std::to_string(rootBone->mNumChildren);
			}




			//skinnedMesh->rootBoneNode = rootBone;
			skinnedMesh->vertices.resize(scene->mMeshes[node->mMeshes[i]]->mNumVertices);

			for (unsigned int j = 0; j < scene->mMeshes[node->mMeshes[i]]->mNumVertices; j++)
			{
				if (scene->mMeshes[node->mMeshes[i]]->HasPositions())
				{
					skinnedMesh->vertices[j].position.x = scene->mMeshes[node->mMeshes[i]]->mVertices[j].x;
					skinnedMesh->vertices[j].position.y = scene->mMeshes[node->mMeshes[i]]->mVertices[j].y;
					skinnedMesh->vertices[j].position.z = scene->mMeshes[node->mMeshes[i]]->mVertices[j].z;
				}

				if (scene->mMeshes[node->mMeshes[i]]->HasNormals())
				{
					skinnedMesh->vertices[j].normal.x = scene->mMeshes[node->mMeshes[i]]->mNormals[j].x;
					skinnedMesh->vertices[j].normal.y = scene->mMeshes[node->mMeshes[i]]->mNormals[j].y;
					skinnedMesh->vertices[j].normal.z = scene->mMeshes[node->mMeshes[i]]->mNormals[j].z;
				}
				if (scene->mMeshes[node->mMeshes[i]]->HasTextureCoords(0)) {
					skinnedMesh->vertices[j].uv.x = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].x;
					skinnedMesh->vertices[j].uv.y = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].y;
				}
			}



			for (unsigned int k = 0; k < scene->mMeshes[node->mMeshes[i]]->mNumBones; k++) {

				unsigned int boneIndex = 0;
				std::string boneName = scene->mMeshes[node->mMeshes[i]]->mBones[k]->mName.C_Str();
				if (skinnedMesh->boneInfoMap.find(boneName) == skinnedMesh->boneInfoMap.end()) {
					boneIndex = skinnedMesh->boneInfoMap.size();
					BoneInfo bi;
					//bi.offsetMatrix = glm::make_mat4(&scene->mMeshes[node->mMeshes[i]]->mBones[k]->mOffsetMatrix.a1);
					bi.offsetMatrix = AssimpMatToGlmMat(scene->mMeshes[node->mMeshes[i]]->mBones[k]->mOffsetMatrix);
					bi.boneKey = boneIndex;
					//bi.nodeName = scene->mMeshes[node->mMeshes[i]]->mBones[k]->mNode->mName.C_Str();
					bi.nodeName = boneName;


					skinnedMesh->boneInfoMap[boneName] = bi;



					glm::vec3 position;
					glm::quat rotation;
					glm::vec3 scale;


					glm::vec3 skew;
					glm::vec4 perspective;

					glm::decompose(bi.offsetMatrix, scale, rotation, position, skew, perspective);


					glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));

					dataToWrite += "\nBone Name:" + bi.nodeName;
					dataToWrite += "\nPosition: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z);
					dataToWrite += "\nRotation: " + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z);
					dataToWrite += "\nScale: " + std::to_string(scale.x) + ", " + std::to_string(scale.y) + ", " + std::to_string(scale.z);
				}
				else {
					boneIndex = std::distance(skinnedMesh->boneInfoMap.begin(), skinnedMesh->boneInfoMap.find(boneName));
				}

				const aiBone* bone = scene->mMeshes[node->mMeshes[i]]->mBones[k];
				for (unsigned int weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
					const aiVertexWeight& weight = bone->mWeights[weightIndex];
					unsigned int vertexID = weight.mVertexId;
					float boneWeight = weight.mWeight;
					skinnedMesh->vertices[vertexID].AddBoneData(boneIndex, boneWeight);
				}
			}


			//BoneInfo* rootBoneInfo = nullptr;
			BoneInfo* rootBoneInfo = &skinnedMesh->boneInfoMap[rootBone->mName.C_Str()];
			skinnedMesh->rootBone = rootBoneInfo;

			/*for (auto& curBone : skinnedMesh->boneInfoMap) {
				if (curBone.first == rootBone->mName.C_Str()) {
					rootBoneInfo = &curBone.second;
					skinnedMesh->rootBone = &curBone.second;
					break;
				}
			}*/


			/*for (auto& curBone : skinnedMesh->boneInfoMap) {
				if (curBone.first == "hand.R") {
					rootBoneInfo = &curBone.second;
					skinnedMesh->rootBone = &curBone.second;
					break;
				}
			}*/
			
			if (rootBoneInfo) {

				for (unsigned int k = 0; k < scene->mMeshes[node->mMeshes[i]]->mNumBones; k++) {

					if (scene->mMeshes[node->mMeshes[i]]->mBones[k]->mName.C_Str() == rootBoneInfo->nodeName) {
						rootBone = scene->mMeshes[node->mMeshes[i]]->mBones[i]->mNode;
					}
				}
			}


			BuildBoneHierarchy(rootBone, skinnedMesh);


			if (scene->mMeshes[node->mMeshes[i]]->mMaterialIndex >= 0) {
				const aiMaterial* material = scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];

				aiString path;
				fs::path fullPath;
				std::string fullPathStr;

				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					skinnedMesh->hasDiffuse = true;

					std::string narrowString = path.C_Str();
					fullPath = fs::path(modelDir) / narrowString;
					fullPathStr = fullPath.string();
					std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

					texLoader.LoadTextureFromFile(startEng.D3DGfx().device, fullPathStr, &skinnedMesh->textures.diffuseTex);
				}
				else {
					skinnedMesh->hasDiffuse = false;
				}


				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {

					skinnedMesh->hasSpecular = true;

					std::string narrowString = path.C_Str();
					fullPath = fs::path(modelDir) / narrowString;
					fullPathStr = fullPath.string();
					std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

					texLoader.LoadTextureFromFile(startEng.D3DGfx().device, fullPathStr, &skinnedMesh->textures.specularTex);
				}
				else {
					skinnedMesh->hasSpecular = false;
				}
			}

			for (unsigned int m = 0; m < scene->mMeshes[node->mMeshes[i]]->mNumFaces; m++)
			{
				const struct aiFace* face = &scene->mMeshes[node->mMeshes[i]]->mFaces[m];
				for (unsigned int k = 0; k < face->mNumIndices; k++)
				{
					skinnedMesh->indices.push_back(face->mIndices[k]);
				}
			}

			if (scene->HasAnimations()) {

				for (unsigned int animIndex = 0; animIndex < scene->mNumAnimations; animIndex++) {
					AnimationData animData;
					const aiAnimation* animation = scene->mAnimations[animIndex];

					animData.name = animation->mName.C_Str();
					animData.duration = animation->mDuration;
					animData.ticksPerSecond = animation->mTicksPerSecond;

					for (unsigned int channelIndex = 0; channelIndex < animation->mNumChannels; channelIndex++) {
						const aiNodeAnim* channel = animation->mChannels[channelIndex];
						AnimationChannel animChannel;
						animChannel.nodeName = std::string(channel->mNodeName.C_Str());



						if (animIndex == 0) {
							dataToWrite += "\n\nChannel:" + animChannel.nodeName;

							unsigned int numPositionKeys = channel->mNumPositionKeys;
							unsigned int numRotationKeys = channel->mNumRotationKeys;
							unsigned int numScaleKeys = channel->mNumScalingKeys;


							dataToWrite += "\nPosition: ";

							for (unsigned int keyframeIndex = 0; keyframeIndex < numPositionKeys; keyframeIndex++) {

								Keyframe keyframe;
								keyframe.time = channel->mPositionKeys[keyframeIndex].mTime;
								keyframe.positionData.position = glm::vec3(channel->mPositionKeys[keyframeIndex].mValue.x, channel->mPositionKeys[keyframeIndex].mValue.y, channel->mPositionKeys[keyframeIndex].mValue.z);
								keyframe.positionData.hasPositionData = true;
								animChannel.keyframes.push_back(keyframe);

								dataToWrite += "\n" + std::to_string(keyframeIndex) + ": " + std::to_string(keyframe.positionData.position.x) + ", " + std::to_string(keyframe.positionData.position.y) + ", " + std::to_string(keyframe.positionData.position.z);
							}

							dataToWrite += "\nRotation: ";
							for (unsigned int keyframeIndex = 0; keyframeIndex < numRotationKeys; keyframeIndex++) {
								Keyframe keyframe;
								keyframe.time = channel->mRotationKeys[keyframeIndex].mTime;
								keyframe.rotationData.rotation = glm::quat(channel->mRotationKeys[keyframeIndex].mValue.w, channel->mRotationKeys[keyframeIndex].mValue.x, channel->mRotationKeys[keyframeIndex].mValue.y, channel->mRotationKeys[keyframeIndex].mValue.z);
								keyframe.rotationData.hasRotationData = true;
								animChannel.keyframes.push_back(keyframe);

								dataToWrite += "\n" + std::to_string(keyframeIndex) + ": W: " + std::to_string(keyframe.rotationData.rotation.w) + ", " + std::to_string(keyframe.rotationData.rotation.x) + ", " + std::to_string(keyframe.rotationData.rotation.y) + ", " + std::to_string(keyframe.rotationData.rotation.z);
							}

							for (unsigned int keyframeIndex = 0; keyframeIndex < numScaleKeys; keyframeIndex++) {
								Keyframe keyframe;
								keyframe.time = channel->mScalingKeys[keyframeIndex].mTime;
								keyframe.scaleData.scale = glm::vec3(channel->mScalingKeys[keyframeIndex].mValue.x, channel->mScalingKeys[keyframeIndex].mValue.y, channel->mScalingKeys[keyframeIndex].mValue.z);
								keyframe.scaleData.hasScaleData = true;
								animChannel.keyframes.push_back(keyframe);

								//dataToWrite += "\nScale on keyframe " + std::to_string(keyframeIndex) + ":" + std::to_string(keyframe.scaleData.scale.x) + ", " + std::to_string(keyframe.scaleData.scale.y) + ", " + std::to_string(keyframe.scaleData.scale.z);
							}
						}


						animData.channels.push_back(animChannel);
					}

					
					
					//SaveAnimationChannelHierarchy(rootBone, &animData);

					skinnedMesh->animations.push_back(animData);

					dataToWrite += "\n\n";
				}


				models->AddSkinnedModel(skinnedMesh);
			}
			
			for (int l = 0; l < skinnedMesh->vertices.size(); l++) {

				skinnedMesh->vertices[l].NormalizeWeights();
			}
			/*for (int l = 0; l < skinnedMesh->vertices.size(); l++) {

				float weight = 0.0f;

				for (int p = 0; p < 4; p++) {

					weight += skinnedMesh->vertices[l].boneWeights[p];
				}

				if (( weight - 0.03f) > 1.0f || (weight + 0.03f) < 1.0f) {
					exit(std::stoi(std::to_string(weight * 1000000)));
				}
			}*/
		}

		std::string dataPath = "D:/SkinnedMeshData.txt";

		WriteDataToFile(dataPath, dataToWrite);

	}
}


void Container::BuildBoneHierarchy(aiNode* boneNode, SkinnedModel* skinnedModel) {

	if (!boneNode || !skinnedModel) {
		dataToWrite += "Error: boneNode or skinnedModel is null.";
		return;
	}

	BoneInfo* curBoneInfo = nullptr;

	auto it = std::find_if(skinnedModel->boneInfoMap.begin(), skinnedModel->boneInfoMap.end(),
		[&](const std::pair<std::string, BoneInfo>& bone) {
			return bone.second.nodeName == boneNode->mName.C_Str();
		});

	if (it != skinnedModel->boneInfoMap.end()) {
		curBoneInfo = &it->second;
	}

	if (!curBoneInfo) {
		curBoneInfo = skinnedModel->rootBone;
		if (!curBoneInfo) {
			dataToWrite += "WTF";
		}
	}

	if (curBoneInfo) {
		dataToWrite += "\n";
		dataToWrite += curBoneInfo->nodeName;
		for (unsigned int i = 0; i < boneNode->mNumChildren; ++i) {
			aiNode* childNode = boneNode->mChildren[i];

			auto childIt = std::find_if(skinnedModel->boneInfoMap.begin(), skinnedModel->boneInfoMap.end(),
				[&](const std::pair<std::string, BoneInfo>& bone) {
					return bone.first == childNode->mName.C_Str();
				});

			if (childIt != skinnedModel->boneInfoMap.end()) {
				BoneInfo* childBoneInfo = &childIt->second;
				curBoneInfo->childBones.push_back(childBoneInfo->nodeName);
				BuildBoneHierarchy(childNode, skinnedModel);
			}
		}
	}
}



void Container::SaveAnimationChannelHierarchy(aiNode* rootBoneNode, AnimationData* animation) {
	std::map<std::string, aiNode*> nodeMap;
	BuildNodeMap(rootBoneNode, nodeMap);

	AnimationChannel rootChannel;
	SaveNodeHierarchy(rootBoneNode, rootChannel, nodeMap);

	animation->channels.push_back(rootChannel);
}

void Container::BuildNodeMap(aiNode* node, std::map<std::string, aiNode*>& nodeMap) {
	nodeMap[node->mName.C_Str()] = node;
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		BuildNodeMap(node->mChildren[i], nodeMap);
	}
}

void Container::SaveNodeHierarchy(aiNode* node, AnimationChannel& channel, const std::map<std::string, aiNode*>& nodeMap) {
	channel.nodeName = node->mName.C_Str();

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		AnimationChannel childChannel;
		SaveNodeHierarchy(node->mChildren[i], childChannel, nodeMap);
		channel.childrenChannels.push_back(new AnimationChannel(childChannel));
	}
}


void Container::CollectBoneNames(const aiScene* scene, std::unordered_set<std::string>& boneNames) {
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int j = 0; j < mesh->mNumBones; ++j) {
			aiBone* bone = mesh->mBones[j];
			boneNames.insert(bone->mName.C_Str());
		}
	}
}

aiNode* Container::FindRootBoneNode(aiNode* node, const std::unordered_set<std::string>& boneNames) {
	if (boneNames.find(node->mName.C_Str()) != boneNames.end()) {
		bool isRoot = true;

		aiNode* parent = node->mParent;
		while (parent) {
			if (boneNames.find(parent->mName.C_Str()) != boneNames.end()) {
				isRoot = false;
				break;
			}
			parent = parent->mParent;
		}

		if (isRoot) {
			return node;
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		aiNode* rootBoneNode = FindRootBoneNode(node->mChildren[i], boneNames);
		if (rootBoneNode != nullptr) {
			return rootBoneNode;
		}
	}

	return nullptr;
}



void Container::LoadHierarchy(aiNode* node, Container* parentContainer, const aiScene* scene, std::string& modelDir) {

	Container* curContainer;

	if (node != scene->mRootNode) {

		curContainer = new Container();
		curContainer->transform.container = curContainer;
		curContainer->transform.parent = &parentContainer->transform;
		parentContainer->transform.children.push_back(&curContainer->transform);
	}
	else {
		curContainer = parentContainer;
	}

	//load actual model
	StoreModels(&curContainer->models, node, scene, modelDir);


	//load transform and store it in curContainer
	glm::mat4 nodeMat = AssimpMatToGlmMat(node->mTransformation);

	glm::vec3 position;
	glm::quat rotation;
	//glm::vec3 rotation;
	glm::vec3 scale;


	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(nodeMat, scale, rotation, position, skew, perspective);

	//rotation = glm::conjugate(rotation);


	glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));


	curContainer->transform.position = Vec3(position.x, position.y, position.z);
	curContainer->transform.quaternionRotation = rotation;
	curContainer->transform.rotation = Vec3(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	curContainer->transform.scale = Vec3(scale.x, scale.y, scale.z);
	//curContainer->transform.scale = Vec3(1.0f, 1.0f, 1.0f);



	curContainer->name = node->mName.C_Str();


	for (unsigned int i = 0; i < node->mNumChildren; ++i) {

		LoadHierarchy(node->mChildren[i], curContainer, scene, modelDir);
	}
}