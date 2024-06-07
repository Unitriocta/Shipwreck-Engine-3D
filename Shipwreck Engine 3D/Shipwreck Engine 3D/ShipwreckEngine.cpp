#include "ShipwreckEngine.h"




#include "StartupGame.h"



#include "Player.h"
#include "ConnectedPlayer.h"
#include "GameGeneration.h"
#include "ConnectionHandler.h"

using namespace GameScripting;


StartupGame startupGame;



//typedef Player* (*CreatePlayerFunc)();
//typedef GameGeneration* (*CreateGameGenerationFunc)();


Player* CreatePlayer() { /*playerInstance = new Player(); */return new Player(); }
ConnectedPlayer* CreateConnectedPlayer() { /*playerInstance = new Player(); */return new ConnectedPlayer(); }
GameGeneration* CreateGameGeneration() { return new GameGeneration(); }
ConnectionHandler* CreateConnectionHandler() { return new ConnectionHandler(); }




namespace GameScripting {

	std::vector<GameScript*> startupScripts = { &startupGame };

	std::map<std::string, std::function<GameScript*()>> scriptFactories =
	{
		{"Player",                CreatePlayer},
		{"ConnectedPlayer",       CreateConnectedPlayer},
		{"GameGeneration",        CreateGameGeneration},
		{"ConnectionHandler",	  CreateConnectionHandler}
		//{"StartupGame",   }
	};

	void AddScriptToGameObject(Container* scriptObject, std::string scriptName) {
		GameScript* script = scriptFactories[scriptName]();
		if (script != nullptr) {
			scriptObject->scripts.push_back(script);
			script->scriptObjects.push_back(scriptObject->name);
		}
	}

	//Player* playerInstance;
}


namespace GameVariables {

	std::mutex playerMutex;
	std::vector<void*> connectionLoopFunctions;
}