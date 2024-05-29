#pragma once

#include "ShipwreckEngine.h"

class StartupGame : public GameScript {
public:
	StartupGame() {
	}

	void Start(Container* container) override;

	void Update(float deltaTime, Container* container) override;

};
