#include "ModelImporter.h"


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "StartEngine.h"
using namespace EngineInstance;


void ModelImporter::ImportModel(Container* rootContainer, const char* filename) {
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
		LoadScene(rootContainer, scene, filename, startEng.D3DGfx().device);
	}
	else {
		LoadScene(rootContainer, scene, filename, nullptr);
	}

}


void ModelImporter::LoadScene(Container* rootContainer, const aiScene* scene, const char* filename, ID3D11Device* device) {
	
	fs::path filepath = filename;
	std::string modelDirectory = filepath.parent_path().string();
	LoadHierarchy(scene->mRootNode, rootContainer, scene, modelDirectory);

	/*for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		Model mesh;
		mesh.vertices.resize(scene->mMeshes[i]->mNumVertices);

		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			if (scene->mMeshes[i]->HasPositions())
			{
				mesh.vertices[j].position.x = scene->mMeshes[i]->mVertices[j].x;
				mesh.vertices[j].position.y = scene->mMeshes[i]->mVertices[j].y;
				mesh.vertices[j].position.z = scene->mMeshes[i]->mVertices[j].z;
			}
			if (scene->mMeshes[i]->HasBones())
			{
				
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

			aiString path;
			fs::path fullPath;
			std::string fullPathStr;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				mesh.hasDiffuse = true;

				std::string narrowString = path.C_Str();
				fullPath = fs::path(modelDirectory) / narrowString;
				fullPathStr = fullPath.string();
				std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

				texLoader.LoadTextureFromFile(device, fullPathStr, &mesh.textures.diffuseTex);
			}
			else {
				mesh.hasDiffuse = false;
			}


			if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {

				mesh.hasSpecular = true;

				std::string narrowString = path.C_Str();
				fullPath = fs::path(modelDirectory) / narrowString;
				fullPathStr = fullPath.string();
				std::replace(fullPathStr.begin(), fullPathStr.end(), '\\', '/');

				texLoader.LoadTextureFromFile(device, fullPathStr, &mesh.textures.specularTex);
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

		rootContainer->models.AddModel(mesh);
	}*/
}

void ModelImporter::StoreModels(Models* models, aiNode* node, const aiScene* scene, std::string& modelDir) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
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
			if (scene->mMeshes[node->mMeshes[i]]->HasBones())
			{

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

		models->AddModel(&mesh);
	}
}

void ModelImporter::LoadHierarchy(aiNode* node, Container* parentContainer, const aiScene* scene, std::string& modelDir) {

	Container* curContainer;
		
	if (node != scene->mRootNode) {

		curContainer = new Container();
		startEng.containers.push_back(curContainer);
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
	curContainer->transform.rotation = Vec3(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	curContainer->transform.scale = Vec3(scale.x, scale.y, scale.z); //Vec3(1.0f, 1.0f, 1.0f);



	curContainer->name = node->mName.C_Str();


	for (unsigned int i = 0; i < node->mNumChildren; ++i) {

		LoadHierarchy(node->mChildren[i], curContainer, scene, modelDir);
	}
}