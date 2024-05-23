#include "ModelImporter.h"

#include "StartEngine.h"
using namespace EngineInstance;



void ModelImporter::ImportModel(const char* filename, D3DTexture* textureLoader) {
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


void ModelImporter::LoadScene(const aiScene* scene, const char* filename, D3DTexture* textureLoader, ID3D11Device* device) {
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
			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				mesh.vertices[j].uv.x = scene->mMeshes[i]->mTextureCoords[0][j].x;
				mesh.vertices[j].uv.y = scene->mMeshes[i]->mTextureCoords[0][j].y;
			}
		}

		if (scene->mMeshes[i]->mMaterialIndex >= 0) {
			const aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

			// Check for diffuse textures (use aiTextureType_SPECULAR for specular textures, etc.)
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				//load path
				mesh.hasDiffuse = true;

				std::string narrowString = path.C_Str();

				ID3D11ShaderResourceView* srv = nullptr;
				textureLoader->LoadTextureFromFile(device, narrowString, &srv);

				mesh.textures.diffuseTex = srv;
			}
			else {
				mesh.hasDiffuse = false;
			}


			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {

				//load path
				mesh.hasSpecular = true;

				std::string narrowString = path.C_Str();

				ID3D11ShaderResourceView* srv = nullptr;
				textureLoader->LoadTextureFromFile(device, narrowString, &srv);

				mesh.textures.specularTex = srv;
			}
			else {
				mesh.hasSpecular = false;
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