#include "ShipwreckEngine.h"


namespace GameScripting {

    ActualGameScript actualGameScript;
    


	std::vector<GameScript*> gameScripts = {&actualGameScript};

	std::map<std::string, GameScript*> nameToScript =
	{ {"ActualGameScript", &actualGameScript}};

	void AddScriptToGameObject(Container* scriptObject, std::string scriptName) {
		scriptObject->scripts.push_back(nameToScript[scriptName]);
		actualGameScript.scriptObjects.push_back(scriptObject->name);
	}
}



void ActualGameScript::Start (Container* container) {
    
    ModelImporter modelImporter;
    Container container1;
    Container container2;
    Container container3;

    D3DTexture texLoader;

    Sprite sprite;

    sprite.LoadTexture("C:/Users/smsal/OneDrive/Documents/Game Art/Mask Game/Character/Character-1.png");

    modelImporter.ImportModel("C:/Users/smsal/Downloads/slime-test-free-texture/source/Scene 3/Scene 3.obj", &texLoader);
    modelImporter.ImportModel("C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx", &texLoader);





    startEng.containers.push_back(&container2);
    startEng.containers.push_back(&container1);
    startEng.containers.push_back(&container3);


    container1.name = "Ball";
    container2.name = "Ground";
    container3.name = "PlayerSprite";

    container1.models.AddModel(modelImporter.meshes[0]);
    container1.models.modelList[0].modelPath = "C:/Users/smsal/OneDrive/Documents/Blender Modules/sphereV2.fbx";

    container2.models.AddModel(modelImporter.meshes[1]);
    container2.models.modelList[0].modelPath = "C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx";

    container3.sprites.AddSprite(&sprite);

    container1.rigidbody = Rigidbody(&physics, true);
    container2.rigidbody = Rigidbody(&physics, false);
    container3.rigidbody = Rigidbody(&physics, true);


    container1.rigidbody.NewRB();
    container2.rigidbody.NewRB();
    container3.rigidbody.NewRB2D();

    startEng.RenderFrame();
}