#pragma once

#include "ShipwreckEngine.h"

class GameGeneration : public GameScript {
public:
	GameGeneration() {
	}

	void Start(Container* container);

	void Update(float deltaTime, Container* container);

	std::vector<std::string> scriptObjects;
};
