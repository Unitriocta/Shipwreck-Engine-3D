#include "Model.h"


#include "StartEngine.h"
using namespace EngineInstance;



void Model::ImportModel(const char* filename, D3DTexture* textureLoader) {
	//clear mesh

	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices);
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
		//error
	}

	if (isD3D) {
		LoadScene(scene, filename, textureLoader, startEng.D3DGfx().device);
	}
	else {
		LoadScene(scene, filename, textureLoader, nullptr);
	}

}


void Model::LoadScene(const aiScene* scene, const char* filename, D3DTexture* textureLoader, ID3D11Device* device) {
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		//Model mesh; // Create a new mesh for each scene mesh
		vertices.resize(scene->mMeshes[i]->mNumVertices);

		// Populate vertex data
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			if (scene->mMeshes[i]->HasPositions())
			{
				vertices[j].position.x = scene->mMeshes[i]->mVertices[j].x;
				vertices[j].position.y = scene->mMeshes[i]->mVertices[j].y;
				vertices[j].position.z = scene->mMeshes[i]->mVertices[j].z;
			}
			if (scene->mMeshes[i]->HasNormals())
			{
				vertices[j].normal.x = scene->mMeshes[i]->mNormals[j].x;
				vertices[j].normal.y = scene->mMeshes[i]->mNormals[j].y;
				vertices[j].normal.z = scene->mMeshes[i]->mNormals[j].z;
			}
			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				vertices[j].uv.x = scene->mMeshes[i]->mTextureCoords[0][j].x;
				vertices[j].uv.y = scene->mMeshes[i]->mTextureCoords[0][j].y;
			}
		}

		if (scene->mMeshes[i]->mMaterialIndex >= 0) {
			const aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

			// Check for diffuse textures (use aiTextureType_SPECULAR for specular textures, etc.)
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				//load path
				hasDiffuse = true;

				std::string narrowString = path.C_Str();

				ID3D11ShaderResourceView* srv = nullptr;
				textureLoader->LoadTextureFromFile(device, narrowString, &srv);

				textures.diffuseTex = srv;
			}
			else {
				hasDiffuse = false;
			}


			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {

				//load path
				hasSpecular = true;

				std::string narrowString = path.C_Str();

				ID3D11ShaderResourceView* srv = nullptr;
				textureLoader->LoadTextureFromFile(device, narrowString, &srv);

				textures.specularTex = srv;
			}
			else {
				hasSpecular = false;
			}
		}

		for (unsigned int m = 0; m < scene->mMeshes[i]->mNumFaces; m++)
		{
			const struct aiFace* face = &scene->mMeshes[i]->mFaces[m];
			for (unsigned int k = 0; k < face->mNumIndices; k++)
			{
				indices.push_back(face->mIndices[k]);
			}
		}
	}
}