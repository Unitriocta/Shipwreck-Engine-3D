#include "ShipwreckEngine.h"




#include "StartupGame.h"



#include "Player.h"
#include "GameGeneration.h"



StartupGame startupGame;



//typedef Player* (*CreatePlayerFunc)();
//typedef GameGeneration* (*CreateGameGenerationFunc)();


Player* CreatePlayer() { return new Player(); }
GameGeneration* CreateGameGeneration() { return new GameGeneration(); }




namespace GameScripting {

	std::vector<GameScript*> startupScripts = { &startupGame };

	std::map<std::string, std::function<GameScript*()>> scriptFactories =
	{
		{"Player",                CreatePlayer},
		{"GameGeneration",        CreateGameGeneration},
		//{"StartupGame",   }
	};

	void AddScriptToGameObject(Container* scriptObject, std::string scriptName) {
		GameScript* script = scriptFactories[scriptName]();
		if (script != nullptr) {
			scriptObject->scripts.push_back(script);
			script->scriptObjects.push_back(scriptObject->name);
		}
	}
}