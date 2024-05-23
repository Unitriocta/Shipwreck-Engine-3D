#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>


class Container;

class GameScript;


namespace GameScripting {
	extern std::map<std::string, GameScript*> nameToScript;


	inline void AddScriptToGameObject(Container* scriptObject, std::string scriptName);
};



#include "StartEngine.h"
using namespace EngineInstance;

#include "Container.h"


//#include "ActualGameScript.h"




class GameScript {
public:
	GameScript() {

	}

	virtual void Update(float deltaTime, Container* container) = 0;

	std::vector<std::string> scriptObjects;
};

class ActualGameScript : public GameScript {
public:
	ActualGameScript() {

	}

	void Update(float deltaTime, Container* container) override {} //Define later

	std::vector<std::string> scriptObjects = {""};
};






