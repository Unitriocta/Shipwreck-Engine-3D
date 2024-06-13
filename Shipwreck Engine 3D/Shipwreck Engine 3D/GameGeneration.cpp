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


	container1.name = "Ball";
	container2.name = "Ground";
	container3.name = "PlayerSprite";


	//modelImporter.ImportModel("C:/Users/smsal/Downloads/slime-test-free-texture/source/Scene 3/Scene 3.obj", &texLoader);
	//modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/handV2.fbx", &texLoader);
	//modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/bow.fbx", &texLoader);
	//modelImporter.ImportModel("D:/Downloads/Vase_Clay.fbx", &texLoader);
	modelImporter.ImportModel(&container1, "D:/Downloads/shooting-building-sci-fi-low-poly/source/3D.fbx");
	//modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx", &texLoader);
	
	////Test Texture Load: Works correctly!
	//std::string newTexPath = "D:/Downloads/shooting-building-sci-fi-low-poly/textures/w (2).jpeg";
	//texLoader.LoadTextureFromFile(startEng.D3DGfx().device, newTexPath, &container1.models.modelList[0].textures.diffuseTex);
	
	//container1.models.modelList[0].modelPath = "C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx";

	modelImporter.ImportModel(&container2, "C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx");
	//container2.models.AddModel(modelImporter.meshes.back());

	container3.sprites.AddSprite(&sprite);

	container1.rigidbody = Rigidbody(&physics, true);
	container2.rigidbody = Rigidbody(&physics, false);
	container3.rigidbody = Rigidbody(&physics, true);


	container1.rigidbody.NewRB();
	container2.rigidbody.NewRB();
	container3.rigidbody.NewRB2D();
}

void GameGeneration::Update(float deltaTime, Container* container) {

}