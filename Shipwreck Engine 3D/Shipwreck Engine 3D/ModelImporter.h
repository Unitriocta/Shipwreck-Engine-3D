#pragma once

#include "MathExtras.h"
#include "Model.h"

#include <d3d11.h>




#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelImporter
{
public:
	void ImportModel(const char* filename) {
		//clear mesh
		
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filename,aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices);

		if (!scene) {
			//error
		}

		LoadScene(scene,filename);
	}

	void LoadScene(const aiScene* scene, const char* filename) {
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			Model mesh; // Create a new mesh for each scene mesh
			mesh.vertices.resize(scene->mMeshes[i]->mNumVertices);

			// Populate vertex data
			for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
			{
				if (scene->mMeshes[i]->HasPositions())
				{
					mesh.vertices[j].position.x = scene->mMeshes[i]->mVertices[j].x;
					mesh.vertices[j].position.y = scene->mMeshes[i]->mVertices[j].y;
					mesh.vertices[j].position.z = scene->mMeshes[i]->mVertices[j].z;
				}
				if (scene->mMeshes[i]->HasNormals())
				{
					mesh.vertices[j].normal.x = scene->mMeshes[i]->mNormals[j].x;
					mesh.vertices[j].normal.y = scene->mMeshes[i]->mNormals[j].y;
					mesh.vertices[j].normal.z = scene->mMeshes[i]->mNormals[j].z;
				}
				/*if (scene->mMeshes[i]->HasTextureCoords(j)) {
					meshes[i].vertices[j].uv.x = scene->mMeshes[i]->mTextureCoords[0][j].x;
					meshes[i].vertices[j].uv.y = scene->mMeshes[i]->mTextureCoords[0][j].y;
				}*/
			}

			if (scene->HasMaterials()) {
				for (unsigned int l = 0; l < scene->mNumMaterials; l++) {
					for (unsigned int m = 0; m < scene->mMaterials[l]->GetTextureCount(aiTextureType_DIFFUSE); m++) {

						aiString path;
						scene->mMaterials[l]->GetTexture(aiTextureType_DIFFUSE, m, &path);

						Texture texture;

						const aiTexture* _texture = scene->GetEmbeddedTexture(path.C_Str());

					}
				}
			}
			
			for (unsigned int m = 0; m < scene->mMeshes[i]->mNumFaces; m++)
			{
				const struct aiFace* face = &scene->mMeshes[i]->mFaces[m];
				for (unsigned int k = 0; k < face->mNumIndices; k++)
				{
					mesh.indices.push_back(face->mIndices[k]);
				}
			}

			meshes.push_back(mesh);
		}
	}

public:
	std::vector<Model> meshes;
	std::vector<Texture> textures;
};

