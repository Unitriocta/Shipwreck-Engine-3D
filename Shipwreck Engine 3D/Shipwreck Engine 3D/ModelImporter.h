#pragma once

class ModelImporter;

#include "MathExtras.h"

#include "TextureMaps.h"

#include "Model.h"


#include <d3d11.h>




#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelImporter
{
public:
	void ImportModel(const char* filename, D3DTexture* textureLoader);

	void LoadScene(const aiScene* scene, const char* filename, D3DTexture* textureLoader, ID3D11Device* device);

public:
	std::vector<Model> meshes;
};

