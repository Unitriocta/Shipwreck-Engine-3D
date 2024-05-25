#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>


class Container;

class GameScript;

class ActualGameScript;

namespace GameScripting {

	//extern ActualGameScript actualGameScript;



	extern std::vector<GameScript*> gameScripts;

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

void Yees();

class HandleConnections  {

	HandleConnections() {
		//startEng.networkManager.stringToVoid = {
		//	//{"PrintHi",      PrintHi},
		//	{"", Yees},
		//};
	}
	
};



class ActualGameScript : public GameScript {
public:
	ActualGameScript() {
		scriptObjects = {"Ball"};
	}
	
	void Start(Container* container) override;

	void Update(float deltaTime, Container* container) override {} //Define later

	/*void SetScriptObjects(std::vector<std::string> scriptObjects_) override {
		scriptObjects = scriptObjects_;
	}

	virtual void AddScriptObject(std::string scriptObject_) {
		scriptObjects.push_back(scriptObject_);
	}*/

};






