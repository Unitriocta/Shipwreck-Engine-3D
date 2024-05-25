#pragma once

#include <iostream>
#include <vector>

#include "TextureMaps.h"

#include "MathExtras.h"

#include <d3d11.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class Model {
public:
	Model(std::vector<TexturedVertex> _vertices, std::vector<unsigned int> _indices)
	{
		vertices = _vertices;
		indices = _indices;
		textures = ModelTextureMaps();
	}

	Model()
	{
		vertices = std::vector<TexturedVertex>();
		indices = std::vector<unsigned int>();
		textures = ModelTextureMaps();
	}


	void ImportModel(const char* filename, D3DTexture* textureLoader);
	void LoadScene(const aiScene* scene, const char* filename, D3DTexture* textureLoader, ID3D11Device* device);



	std::vector<TexturedVertex> vertices;
	std::vector<unsigned int> indices; //change to unsigned int for OpenGL, unsigned short for DirectX11


	ModelTextureMaps textures;

	std::string modelPath;

	bool hasSpecular;
	bool hasDiffuse;
};