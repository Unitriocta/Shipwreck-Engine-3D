#include "GameGeneration.h"

#include "StartEngine.h"
using namespace EngineInstance;

Container container1;
Container container2;
Container spriteContainer;

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
	//container1.ImportModel("D:/Downloads/BGE_Building_1-03/BGE_Building 1-03.fbx");
	//container1.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/BGE_Building 1-03.fbx");
	container2.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/handV2.fbx");
	//container2.ImportModel("D:/Downloads/test-ani/source/testAni.fbx");


	/*spriteContainer.sprites.AddSprite(&sprite);
	spriteContainer.transform.position = Vec3(0.0f, 5.0f, 5.0f);
	spriteContainer.transform.updateGlobalProperties();
	spriteContainer.rigidbody.NewRB(true, true);*/
	container2.transform.position = Vec3(-200.0f, 0.0f, 0.0f);
	for (int i = 0; i < container1.transform.children.size(); i++) {
		//container1.transform.children[i]->container->rigidbody.NewRB(false, false);
	}
	container1.rigidbody.NewRB(false, false);
	//container2.rigidbody.NewRB();
	//container3.rigidbody.NewRB2D();

	std::string dataPath = "D:/AnimationName";
	std::string writeData = "";

	//writeData += container2.transform.children[1]->container->models.skinnedModelList[0].animations[0].name;


	WriteDataToFile(dataPath, writeData);
}

void FindAnimation(Container* container, float deltaTime) {

	for (int i = 0; i < container->models.skinnedModelList.size(); i++) {
		if (container->models.skinnedModelList[i].animations.size() > 0) {
			container->models.skinnedModelList[i].UpdateAnimation(deltaTime);
		}
	}

	for (int i = 0; i < container->transform.children.size(); i++) {

		FindAnimation(container->transform.children[i]->container, deltaTime);
	}
}



void GameGeneration::Update(float deltaTime, Container* container) {
	//container2.transform.children[1]->container->models.skinnedModelList[0].UpdateAnimation(deltaTime);
	FindAnimation(&container2, deltaTime);
	//container2.transform.children[1]->container->models.skinnedModelList[0].UpdateAnimation(deltaTime);

	if (input.xrInput->buttonA || input.xrInput->buttonB || input.xrInput->menuButton) {
		exit(14324);
	}
}