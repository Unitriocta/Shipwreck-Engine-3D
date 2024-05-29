#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include <functional>

class Container;

class GameScript;




namespace GameScripting {

	extern std::vector<GameScript*> startupScripts;

	extern std::map<std::string, std::function<GameScript*()>> scriptFactories;

	void AddScriptToGameObject(Container* scriptObject, std::string scriptName);
}


#include "StartEngine.h"
using namespace EngineInstance;

#include "Container.h"



class GameScript {
public:
	GameScript() {
	}

	virtual void Start(Container* container) = 0;

	virtual void Update(float deltaTime, Container* container) = 0;

	void SetScriptObjects(std::vector<std::string>& scriptObjects_) {
		scriptObjects = scriptObjects_;
	}

	void AddScriptObject(std::string scriptObject_) {
		scriptObjects.push_back(scriptObject_);

	}

	std::vector<std::string> scriptObjects;
};
