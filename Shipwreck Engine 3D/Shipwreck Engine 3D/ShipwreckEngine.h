#pragma once

#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <memory>
#include <vector>

#include <functional>

class Container;

#include "GameScript.h"
//class GameScript;




namespace GameScripting {

	extern std::vector<GameScript*> startupScripts;

	extern std::map<std::string, std::function<GameScript*()>> scriptFactories;

	void AddScriptToGameObject(Container* scriptObject, std::string scriptName);
}


namespace GameVariables {

	extern std::mutex playerMutex;
	extern std::vector<void*> connectionLoopFunctions;
}


#include "StartEngine.h"
using namespace EngineInstance;



//class GameScript {
//public:
//	GameScript() {
//	}
//
//	virtual void Start(Container* container) = 0;
//
//	virtual void Update(float deltaTime, Container* container) = 0;
//
//	void SetScriptObjects(std::vector<std::string>& scriptObjects_) {
//		scriptObjects = scriptObjects_;
//	}
//
//	void AddScriptObject(std::string scriptObject_) {
//		scriptObjects.push_back(scriptObject_);
//
//	}
//
//	std::vector<std::string> scriptObjects;
//};
