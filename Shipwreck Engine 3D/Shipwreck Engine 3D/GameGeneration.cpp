#include "GameGeneration.h"

#include "StartEngine.h"
using namespace EngineInstance;

Container container1;
Container container2;

Sprite sprite;

void GameGeneration::Start(Container* container) {
	

	sprite.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Mask Game/Character/Character-1.png");









	//modelImporter.ImportModel(&container1, "C:/Users/smsal/Downloads/slime-test-free-texture/source/Scene 3/Scene 3.obj");
	//modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/handV2.fbx", &texLoader);z
	//modelImporter.ImportModel(&container1, "C:/Users/smsal/OneDrive/Documents/Blender Modules/bow.fbx");
	//modelImporter.ImportModel(&container1, "D:/Downloads/Vase_Clay.fbx");
	//modelImporter.ImportModel(&container1, "D:/Downloads/shooting-building-sci-fi-low-poly/source/3D.fbx");
	//modelImporter.ImportModel(&container1, "C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx");
	
	//container1.models.modelList[0].modelPath = "C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx";

	//container1.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx");
	//container1.ImportModel("D:/Downloads/shooting-building-sci-fi-low-poly/source/3D.fbx");
	//container1.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx");
	container2.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/handV2.fbx");
	//container2.ImportModel("D:/Downloads/test-ani/source/testAni.fbx");


	//container3.sprites.AddSprite(&sprite);

	for (int i = 0; i < container1.transform.children.size(); i++) {
		//container1.transform.children[i]->container->rigidbody.NewRB(false, false);
	}
	container1.rigidbody.NewRB(false, false);
	//container2.rigidbody.NewRB();
	//container3.rigidbody.NewRB2D();

	std::string dataPath = "D:/AnimationName";
	std::string writeData = "";

	writeData += container2.transform.children[1]->container->models.skinnedModelList[0].animations[0].name;


	WriteDataToFile(dataPath, writeData);
}

void GameGeneration::Update(float deltaTime, Container* container) {
	container2.transform.children[1]->container->models.skinnedModelList[0].UpdateAnimation(deltaTime);
}