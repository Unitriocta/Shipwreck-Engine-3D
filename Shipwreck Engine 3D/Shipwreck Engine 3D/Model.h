#pragma once

#include <iostream>
#include <vector>

#include "TextureMaps.h"

#include "MathExtras.h"




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

	std::vector<TexturedVertex> vertices;
	std::vector<unsigned int> indices; //change to unsigned int for OpenGL, unsigned short for DirectX11


	ModelTextureMaps textures;

	std::string modelPath;

	bool hasSpecular;
	bool hasDiffuse;
};