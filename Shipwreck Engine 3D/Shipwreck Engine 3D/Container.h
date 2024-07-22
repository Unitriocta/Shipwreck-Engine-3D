#pragma once

#include <iostream>
#include <vector>
#include <type_traits>

#include "Transform.h"
#include "Camera.h"
#include "Model.h"
#include "Models.h"

#include "Rigidbody.h"

class Sprites;
#include "Sprites.h"

//#include "ShipwreckEngine.h"
class GameScript;
#include "GameScript.h"

#include <unordered_set>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

class Container
{
public:
	Container();
	template <typename Element>
	void AddElement(Element element) {
		if (typeid(element) == typeid(Camera)) {
			camera = dynamic_cast<Camera*>(element);
		}
		else if (typeid(element) == typeid(Transform)) {
			transform = dynamic_cast<Transform*>(element);
		}
		else if (typeid(element) == typeid(Models)) {
			models = dynamic_cast<Models*>(element);
		}
	}



	void ImportModel(const char* filename);

private:
	void LoadScene(Container* rootContainer, const aiScene* scene, const char* filename, ID3D11Device* device);

	void StoreModels(Models* models, aiNode* node, const aiScene* scene, std::string& modelDir);

	void LoadHierarchy(aiNode* node, Container* parentContainer, const aiScene* scene, std::string& modelDir);


	void SaveAnimationChannelHierarchy(aiNode* rootBoneNode, AnimationData* animation);
	void BuildNodeMap(aiNode* node, std::map<std::string, aiNode*>& nodeMap);
	void SaveNodeHierarchy(aiNode* node, AnimationChannel& channel, const std::map<std::string, aiNode*>& nodeMap);


	void CollectBoneNames(const aiScene* scene, std::unordered_set<std::string>& boneNames);

	aiNode* FindRootBoneNode(aiNode* node, const std::unordered_set<std::string>& boneNames);


	void BuildBoneHierarchy(aiNode* boneNode, SkinnedModel* skinnedModel);


	glm::mat4 AssimpMatToGlmMat(const aiMatrix4x4& from) {
		glm::mat4 to;
		to[0][0] = from.a1; to[0][1] = from.b1; to[0][2] = from.c1; to[0][3] = from.d1;
		to[1][0] = from.a2; to[1][1] = from.b2; to[1][2] = from.c2; to[1][3] = from.d2;
		to[2][0] = from.a3; to[2][1] = from.b3; to[2][2] = from.c3; to[2][3] = from.d3;
		to[3][0] = from.a4; to[3][1] = from.b4; to[3][2] = from.c4; to[3][3] = from.d4;
		return to;
	}


	Assimp::Importer* importer;

public:                      //Necessary:
	std::string name;

	Models models;            //N
	Transform transform;      //Y
	Camera camera;            //N
	Rigidbody rigidbody;      //N
	Sprites sprites;
	std::vector<GameScript*> scripts;
	std::vector<std::string> scriptNames;
};