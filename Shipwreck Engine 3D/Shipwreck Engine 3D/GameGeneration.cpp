#include "GameGeneration.h"

#include "StartEngine.h"
using namespace EngineInstance;

Container container1;
Container container2;
Container container3;

Sprite sprite;

void GameGeneration::Start(Container* container) {
	

	sprite.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Mask Game/Character/Character-1.png");





	startEng.containers.push_back(&container2);
	startEng.containers.push_back(&container1);
	startEng.containers.push_back(&container3);




	//modelImporter.ImportModel(&container1, "C:/Users/smsal/Downloads/slime-test-free-texture/source/Scene 3/Scene 3.obj");
	//modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/handV2.fbx", &texLoader);
	//modelImporter.ImportModel(&container1, "C:/Users/smsal/OneDrive/Documents/Blender Modules/bow.fbx");
	//modelImporter.ImportModel(&container1, "D:/Downloads/Vase_Clay.fbx");
	//modelImporter.ImportModel(&container1, "D:/Downloads/shooting-building-sci-fi-low-poly/source/3D.fbx");
	//modelImporter.ImportModel(&container1, "C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx");
	container1.transform.scale = Vec3(1.0f, 1.0f, 1.0f);
	////Test Texture Load: Works correctly!
	//std::string newTexPath = "D:/Downloads/shooting-building-sci-fi-low-poly/textures/w (2).jpeg";
	//texLoader.LoadTextureFromFile(startEng.D3DGfx().device, newTexPath, &container1.models.modelList[0].textures.diffuseTex);
	
	//container1.models.modelList[0].modelPath = "C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx";

	modelImporter.ImportModel(&container2, "C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx");
	container2.transform.scale = Vec3(1.0f, 1.0f, 1.0f);
	//container2.models.AddModel(modelImporter.meshes.back());

	container1.name = "Ball";
	container2.name = "Ground";
	container3.name = "PlayerSprite";

	container3.sprites.AddSprite(&sprite);

	container1.rigidbody.isDynamic = true;
	container2.rigidbody.isDynamic = false;
	container3.rigidbody.isDynamic = true;


	container1.rigidbody.NewRB();
	container2.rigidbody.NewRB();
	container3.rigidbody.NewRB2D();

	if (container1.models.modelList.size() > 0) {
		//container1.rigidbody.UpdateMesh(&container1.models.modelList.back());
	}
	for (int i = 0; i < container1.transform.children.size(); i++) {
		if (container1.transform.children[i]->container->models.modelList.size() > 0) {
			//container1.rigidbody.UpdateMesh(&container1.transform.children[i]->container->models.modelList.back());
			break;
		}
	}
	if (container2.models.modelList.size() > 0) {
		container2.rigidbody.UpdateMesh(&container2.models.modelList.back());
	}
	for (int i = 0; i < container2.transform.children.size(); i++) {
		if (container2.transform.children[i]->container->models.modelList.size() > 0) {
			container2.rigidbody.UpdateMesh(&container2.transform.children[i]->container->models.modelList.back());
			break;
		}
	}
}

void GameGeneration::Update(float deltaTime, Container* container) {

}