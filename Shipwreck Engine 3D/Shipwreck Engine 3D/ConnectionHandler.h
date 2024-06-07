#pragma once

#include "ShipwreckEngine.h"

class ConnectionHandler : public GameScript {
public:
	ConnectionHandler() {
	}

	void Start(Container* container) override;

	void Update(float deltaTime, Container* container) override;



	void DelayFrameSend();
	void DelayInitializationSend();



	std::map<int, Transform*> playerNumToTransform;
};
