#pragma once

#include <iostream>
#include <string>
class Container;
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