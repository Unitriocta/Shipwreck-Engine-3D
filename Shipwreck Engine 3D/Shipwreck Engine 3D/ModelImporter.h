#pragma once

class ModelImporter;

#include "MathExtras.h"

#include "TextureMaps.h"

#include "Model.h"
#include "Models.h"



#include <d3d11.h>


class Container;
#include "Container.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>


class ModelImporter
{
public:
	void ImportModel(Container* rootContainer, const char* filename);

	void LoadScene(Container* rootContainer, const aiScene* scene, const char* filename, ID3D11Device* device);

	void LoadHierarchy(aiNode* node, Container* parentContainer, const aiScene* scene, std::string& modelDir);
    
    void StoreModels(Models* models, aiNode* node, const aiScene* scene, std::string& modelDir);

    glm::mat4 AssimpMatToGlmMat(const aiMatrix4x4& from) {
        glm::mat4 to;
        to[0][0] = from.a1; to[0][1] = from.b1; to[0][2] = from.c1; to[0][3] = from.d1;
        to[1][0] = from.a2; to[1][1] = from.b2; to[1][2] = from.c2; to[1][3] = from.d2;
        to[2][0] = from.a3; to[2][1] = from.b3; to[2][2] = from.c3; to[2][3] = from.d3;
        to[3][0] = from.a4; to[3][1] = from.b4; to[3][2] = from.c4; to[3][3] = from.d4;
        return to;
    }

public:
	std::vector<Model> meshes;
};

