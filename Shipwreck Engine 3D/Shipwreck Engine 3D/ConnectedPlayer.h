#pragma once

#include "ShipwreckEngine.h"

class ConnectedPlayer : public GameScript {
public:
	ConnectedPlayer() {
	}

	void Start(Container* container) override;

	void Update(float deltaTime, Container* container) override;

	Container* playerConnection;

	int connectedPlayerNum;
};
