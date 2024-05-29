#include "StartupGame.h"

#include "StartEngine.h"
using namespace EngineInstance;

Container startContainer;

void StartupGame::Start(Container* container) {
	
	startEng.containers.push_back(&startContainer);

	startContainer.scriptNames.push_back("Player");
	startContainer.scriptNames.push_back("GameGeneration");
}

void StartupGame::Update(float deltaTime, Container* container) {

}