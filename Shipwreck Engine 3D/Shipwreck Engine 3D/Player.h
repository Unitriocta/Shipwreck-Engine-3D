#pragma once

#include "ShipwreckEngine.h"

class Player : public GameScript {
public:
	Player() {
	}

	void Start(Container* container) override;

	void Update(float deltaTime, Container* container) override;

};
