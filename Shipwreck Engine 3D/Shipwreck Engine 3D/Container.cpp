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
}




void Container::ImportModel(const char* filename) {

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename,
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


void Container::LoadScene(Container* rootContainer, const aiScene* scene, const char* filename, ID3D11Device* device) {

	fs::path filepath = filename;
	std::string modelDirectory = filepath.parent_path().string();
	LoadHierarchy(scene->mRootNode, rootContainer, scene, modelDirectory);
}

void Container::StoreModels(Models* models, aiNode* node, const aiScene* scene, std::string& modelDir) {
	
	std::string dataToWrite;
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

			models->AddModel(mesh);
		}
		else {

			SkinnedModel skinnedMesh;
			skinnedMesh.rootBoneNode = scene->mRootNode;
			skinnedMesh.vertices.resize(scene->mMeshes[node->mMeshes[i]]->mNumVertices);

			for (unsigned int j = 0; j < scene->mMeshes[node->mMeshes[i]]->mNumVertices; j++)
			{
				if (scene->mMeshes[node->mMeshes[i]]->HasPositions())
				{
					skinnedMesh.vertices[j].position.x = scene->mMeshes[node->mMeshes[i]]->mVertices[j].x;
					skinnedMesh.vertices[j].position.y = scene->mMeshes[node->mMeshes[i]]->mVertices[j].y;
					skinnedMesh.vertices[j].position.z = scene->mMeshes[node->mMeshes[i]]->mVertices[j].z;
				}

				if (scene->mMeshes[node->mMeshes[i]]->HasNormals())
				{
					skinnedMesh.vertices[j].normal.x = scene->mMeshes[node->mMeshes[i]]->mNormals[j].x;
					skinnedMesh.vertices[j].normal.y = scene->mMeshes[node->mMeshes[i]]->mNormals[j].y;
					skinnedMesh.vertices[j].normal.z = scene->mMeshes[node->mMeshes[i]]->mNormals[j].z;
				}
				if (scene->mMeshes[node->mMeshes[i]]->HasTextureCoords(0)) {
					skinnedMesh.vertices[j].uv.x = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].x;
					skinnedMesh.vertices[j].uv.y = scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0][j].y;
				}
			}

			
			dataToWrite += "\nBoneData:";
			dataToWrite += "\nNumBones: ";
			dataToWrite += std::to_string(scene->mMeshes[node->mMeshes[i]]->mNumBones);
			dataToWrite += "\n";

			for (unsigned int k = 0; k < scene->mMeshes[node->mMeshes[i]]->mNumBones; k++) {
				
				unsigned int boneIndex = 0;
				std::string boneName = scene->mMeshes[node->mMeshes[i]]->mBones[k]->mName.C_Str();
				if (skinnedMesh.boneInfoMap.find(boneName) == skinnedMesh.boneInfoMap.end()) {
					boneIndex = skinnedMesh.boneInfoMap.size();
					BoneInfo bi;
					//bi.offsetMatrix = glm::make_mat4(&scene->mMeshes[node->mMeshes[i]]->mBones[k]->mOffsetMatrix.a1);
					bi.offsetMatrix = AssimpMatToGlmMat(scene->mMeshes[node->mMeshes[i]]->mBones[k]->mOffsetMatrix);
					bi.boneKey = boneIndex;
					skinnedMesh.boneInfoMap[boneName] = bi;



					glm::vec3 position;
					glm::quat rotation;
					glm::vec3 scale;


					glm::vec3 skew;
					glm::vec4 perspective;

					glm::decompose(bi.offsetMatrix, scale, rotation, position, skew, perspective);


					glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));


					dataToWrite += "\nBone Name: ";
					dataToWrite += boneName;
					dataToWrite += ",";
					dataToWrite += "\nBone Position: ";
					dataToWrite += std::to_string(position.x) + ", ";
					dataToWrite += std::to_string(position.y) + ", ";
					dataToWrite += std::to_string(position.z) + ", ";
					dataToWrite += "\n";
					dataToWrite += "\nBone Rotation: ";
					dataToWrite += std::to_string(eulerAngles.x) + ", ";
					dataToWrite += std::to_string(eulerAngles.y) + ", ";
					dataToWrite += std::to_string(eulerAngles.z) + ", ";
					dataToWrite += "\n";
					dataToWrite += "\nBone Scale: ";
					dataToWrite += std::to_string(scale.x) + ", ";
					dataToWrite += std::to_string(scale.y) + ", ";
					dataToWrite += std::to_string(scale.z) + ", ";
					dataToWrite += "\n";

				}
				else {
					boneIndex = std::distance(skinnedMesh.boneInfoMap.begin(), skinnedMesh.boneInfoMap.find(boneName));
				}
				skinnedMesh.bones.push_back(scene->mMeshes[node->mMeshes[i]]->mBones[k]);

				const aiBone* bone = scene->mMeshes[node->mMeshes[i]]->mBones[k];
				for (unsigned int weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
					const aiVertexWeight& weight = bone->mWeights[weightIndex];
					unsigned int vertexID = weight.mVertexId;
					float boneWeight = weight.mWeight;
					skinnedMesh.vertices[vertexID].AddBoneData(boneIndex, boneWeight);
				}
			}

			dataToWrite += "\n\n";

			if (scene->mMeshes[node->mMeshes[i]]->mMaterialIndex >= 0) {
				const aiMaterial* material = scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];

				aiString path;
				fs::path fullPath;
				std::string fullPathStr;

				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					skinnedMesh.hasDiffuse = true;

					std::string narrowString = path.C_Str();
					fullPath = fs::path(modelDir) / narrowString;
					fullPathStr = fullPath.string();
					std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

					texLoader.LoadTextureFromFile(startEng.D3DGfx().device, fullPathStr, &skinnedMesh.textures.diffuseTex);
				}
				else {
					skinnedMesh.hasDiffuse = false;
				}


				if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {

					skinnedMesh.hasSpecular = true;

					std::string narrowString = path.C_Str();
					fullPath = fs::path(modelDir) / narrowString;
					fullPathStr = fullPath.string();
					std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

					texLoader.LoadTextureFromFile(startEng.D3DGfx().device, fullPathStr, &skinnedMesh.textures.specularTex);
				}
				else {
					skinnedMesh.hasSpecular = false;
				}
			}

			for (unsigned int m = 0; m < scene->mMeshes[node->mMeshes[i]]->mNumFaces; m++)
			{
				const struct aiFace* face = &scene->mMeshes[node->mMeshes[i]]->mFaces[m];
				for (unsigned int k = 0; k < face->mNumIndices; k++)
				{
					skinnedMesh.indices.push_back(face->mIndices[k]);
				}
			}

			if (scene->HasAnimations()) {
				dataToWrite += "\nAnimation Data: ";

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

						unsigned int numPositionKeys = channel->mNumPositionKeys;
						unsigned int numRotationKeys = channel->mNumRotationKeys;
						unsigned int numScalingKeys = channel->mNumScalingKeys;

						unsigned int numKeys = std::min({ numPositionKeys, numRotationKeys, numScalingKeys });

						for (unsigned int keyframeIndex = 0; keyframeIndex < numKeys; keyframeIndex++) {
							Keyframe keyframe;
							keyframe.time = channel->mPositionKeys[keyframeIndex].mTime;
							keyframe.position = glm::vec3(channel->mPositionKeys[keyframeIndex].mValue.x, channel->mPositionKeys[keyframeIndex].mValue.y, channel->mPositionKeys[keyframeIndex].mValue.z);
							keyframe.rotation = glm::quat(channel->mRotationKeys[keyframeIndex].mValue.w, channel->mRotationKeys[keyframeIndex].mValue.x, channel->mRotationKeys[keyframeIndex].mValue.y, channel->mRotationKeys[keyframeIndex].mValue.z);
							keyframe.scale = glm::vec3(channel->mScalingKeys[keyframeIndex].mValue.x, channel->mScalingKeys[keyframeIndex].mValue.y, channel->mScalingKeys[keyframeIndex].mValue.z);
							animChannel.keyframes.push_back(keyframe);

							if ((keyframeIndex == 0 || keyframeIndex == 1) && animIndex == 0 && (channelIndex == 0 || channelIndex == 1)) {
								dataToWrite += "\nKeyframe " + std::to_string(keyframeIndex) + ": ";
								dataToWrite += channel->mNodeName.C_Str();
								dataToWrite += "\nPosition: " + std::to_string(keyframe.position.x) + ", " + std::to_string(keyframe.position.y) + ", " + std::to_string(keyframe.position.z);
								dataToWrite += "\nRotation: " + std::to_string(keyframe.rotation.w) + ", " + std::to_string(keyframe.rotation.x) + ", " + std::to_string(keyframe.rotation.y) + ", " + std::to_string(keyframe.rotation.z);
								dataToWrite += "\nScale: " + std::to_string(keyframe.scale.x) + ", " + std::to_string(keyframe.scale.y) + ", " + std::to_string(keyframe.scale.z);
							}
						}

						animData.channels.push_back(animChannel);
					}

					skinnedMesh.animations.push_back(animData);
				}
				dataToWrite += "\n\n";
			}


			models->AddSkinnedModel(skinnedMesh);
		}
	}
	
	std::string dataPath = "D:/SkinnedMeshData";
	
	WriteDataToFile(dataPath, dataToWrite);
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