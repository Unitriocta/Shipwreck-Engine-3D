#include "Online.h"


#include "StartEngine.h"
using namespace EngineInstance;


void PrintHi() {
	std::cout << "\nHi!!!";
}

void PrintInt(int in) {
	std::cout << "\nInt: " << in;
}

void PrintFloat(float in) {
	std::cout << "\Float: " << in;
}

void PrintString(std::string in) {
	std::cout << "\nString: " << in;
}

void SetPosition(Vec3 in) {

}

void SetPlayerPosition(Vec3 in, int inPlayerNum, NetworkManager* networkManager) {
	networkManager->transformAddresses[inPlayerNum]->position = in;
}



void Server::HandleMessage(const char* data, std::size_t length) {

	startEng.networkManager.HandleMessage(startEng.networkManager.BufferToInfo(data));
}


void Client::HandleMessage(const char* data, std::size_t length) {

	startEng.networkManager.HandleMessage(startEng.networkManager.BufferToInfo(data));
}