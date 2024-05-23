#pragma once

#include <d3d11.h>
#include <directxmath.h>


class ModelTextureMaps
{
public:
	ModelTextureMaps(ID3D11ShaderResourceView* specular, ID3D11ShaderResourceView* diffuse, ID3D11ShaderResourceView* normal, ID3D11ShaderResourceView* regular) :
		specularTex(specular), diffuseTex(diffuse), normalTex(normal), regularTex(regular)
	{}
	ModelTextureMaps() :
		specularTex(nullptr), diffuseTex(nullptr), normalTex(nullptr), regularTex(nullptr)
	{}

	ID3D11ShaderResourceView* specularTex;
	ID3D11ShaderResourceView* diffuseTex;
	ID3D11ShaderResourceView* normalTex;
	ID3D11ShaderResourceView* regularTex;
};