#include "ConnectionHandler.h"

#include "StartEngine.h"

#include "ConnectedPlayer.h"

using namespace EngineInstance;

using namespace GameScripting;

using namespace GameVariables;


Container* playerContainer;

Transform* playerTransform;

std::string positionData;
std::string rotationData;
std::string newConnectionData;
std::string requestNumData;
std::string sendNumData;
//std::string initData;


std::mutex inDelayMutex;
std::mutex inInitDelayMutex;
bool inPositionThread;
bool inInitThread;

float lastPositionSend;
float lastInitSend;

ConnectionHandler* refHandler;

bool initializedConnection;

int clientPlayerNum = 0; //0 means no player num

std::vector<int> allClientNums;

typedef std::string (*NoInFunctionCall)();


//This function works
std::string SendLocalPosition() {

	positionData = "";
	positionData += "{SendPosition:";
	positionData += std::to_string(clientPlayerNum);
	positionData += ":";
	positionData += std::to_string(playerContainer->transform.position.x);
	positionData += ":";
	positionData += std::to_string(playerContainer->transform.position.y);
	positionData += ":";
	positionData += std::to_string(playerContainer->transform.position.z);
	positionData += ":}";

	return positionData;
	//networkManager.client.StartSendThread(positionData);
}

//This function works
std::string SendLocalRotation() {

	rotationData = "";
	rotationData += "{SendRotation:";
	rotationData += std::to_string(clientPlayerNum);
	rotationData += ":";
	rotationData += std::to_string(playerContainer->transform.rotation.x);
	rotationData += ":";
	rotationData += std::to_string(playerContainer->transform.rotation.y);
	rotationData += ":";
	rotationData += std::to_string(playerContainer->transform.rotation.z);
	rotationData += ":}";

	return rotationData;
	//networkManager.client.StartSendThread(rotationData);
}



//Actual join function, initialize data for the newest client
//Change to TCP/UDP Reliable
void AddConnection(int newPlayerNum) {

	if (networkManager.isServer) {

		allClientNums.push_back(newPlayerNum);

		newConnectionData = "";
		newConnectionData += "{AddConnection:";
		newConnectionData += std::to_string(newPlayerNum);
		newConnectionData += ":}";

		networkManager.server.StartSendThread(newConnectionData, true);


		for (int i = 0; i < allClientNums.size(); i++) {

			newConnectionData = "";
			newConnectionData += "{AddConnection:";
			newConnectionData += std::to_string(allClientNums[i]);
			newConnectionData += ":}";

			networkManager.server.StartSendThread(newConnectionData, false);
		}

	}

	if (newPlayerNum != clientPlayerNum && refHandler->playerNumToTransform[newPlayerNum] == nullptr) {

		Container* newPlayer = new Container();
		startEng.containers.push_back(newPlayer);

		modelImporter.ImportModel(newPlayer, "C:/Users/smsal/OneDrive/Documents/Blender Modules/cube.fbx");

		newPlayer->scriptNames.push_back("ConnectedPlayer");

		for (int i = 0; i < startEng.containers.size(); i++) {
			for (int j = 0; j < startEng.containers[i]->scriptNames.size(); j++) {
				if (scriptFactories[startEng.containers[i]->scriptNames[j]] != nullptr && j >= startEng.containers[i]->scripts.size()) {
					AddScriptToGameObject(startEng.containers[i], startEng.containers[i]->scriptNames[j]);
					startEng.containers[i]->scripts.back()->Start(startEng.containers[i]);
				}
			}
		}

		for (int i = 0; i < newPlayer->scriptNames.size(); i++) {
			if (newPlayer->scriptNames[i] == "ConnectedPlayer") {
				ConnectedPlayer* newConnectedPlayer = reinterpret_cast<ConnectedPlayer*>(newPlayer->scripts[i]);

				newConnectedPlayer->playerConnection = newPlayer;
				newConnectedPlayer->connectedPlayerNum = newPlayerNum;

				refHandler->playerNumToTransform[newPlayerNum] = &newPlayer->transform;
				break;
			}
		}
	}
}


//Getting rid of the code within this function get's rid of the problem
//Change to TCP/UDP Reliable
void RequestPlayerNum() {
	if (networkManager.isServer) {
		sendNumData = "";
		sendNumData += "{SendPlayerNum:";
		
		int newPlayerNum = allClientNums.size() + 2;

		/*for (int i = 0; i <= allClientNums.size(); i++ ) {
			newPlayerNum++;

			if (allClientNums[i] == newPlayerNum) {
				break;
			}
		}*/

		sendNumData += std::to_string(newPlayerNum); //When this is std::to_string, the crash is server side, when it is just '7', the crash is client side
		sendNumData += ":}";
		
		//Getting rid of just this line fixes the issue
		networkManager.server.StartSendThread(sendNumData, false);
	}
}

//Getting rid of the code within this function get's rid of the problem
//Change to TCP/UDP Reliable
void SendPlayerNum(int newPlayerNum) {
	if (!networkManager.isServer) {
		clientPlayerNum = newPlayerNum;
	}
}



//Change to TCP/UDP Reliable
//Get rid of player container entirely
void RemoveConnection(int removePlayerNum) {
	//refHandler->playerNumToTransform[removePlayerNum] = nullptr;
}


void ConnectionHandler::DelayFrameSend() {

	if (playerContainer != nullptr) {

		{
			std::lock_guard<std::mutex> guard(inDelayMutex);
			inPositionThread = true;
		}

		auto now = std::chrono::system_clock::now();

		auto wakeUpTime = now + std::chrono::duration<float, std::milli>(1000.0f / 60.0f);

		std::this_thread::sleep_until(wakeUpTime);
		
		std::string allFrameCalls;
		for (int i = 0; i < connectionLoopFunctions.size(); i++) {

			NoInFunctionCall func = reinterpret_cast<NoInFunctionCall>(connectionLoopFunctions[i]);
			allFrameCalls += func();
		}
		networkManager.client.StartSendThread(allFrameCalls);

		lastPositionSend = startEng.timeManager.GetTime();
		{
			std::lock_guard<std::mutex> guard(inDelayMutex);
			inPositionThread = false;
		}
	}
}

void ConnectionHandler::DelayInitializationSend() {

	{
		std::lock_guard<std::mutex> guard(inInitDelayMutex);
		inInitThread = true;
	}

	auto now = std::chrono::system_clock::now();

	auto wakeUpTime = now + std::chrono::duration<float, std::milli>(1000.0f / 60.0f);

	std::this_thread::sleep_until(wakeUpTime);

	//Do Send
	{
		std::lock_guard<std::mutex> guard(inInitDelayMutex);

		if (clientPlayerNum == 0 && !networkManager.isServer) {

			requestNumData = "";
			requestNumData += "{RequestPlayerNum:}";

			networkManager.client.StartSendThread(requestNumData);
		}
		else if (!initializedConnection && !networkManager.isServer) {
			//IMPORTANT: DO CHECKS LATER ON TO SEE IF CONNECTION AND DATA SEND WAS A SUCCESS
			newConnectionData = "";
			newConnectionData += "{AddConnection:";
			newConnectionData += std::to_string(clientPlayerNum);
			newConnectionData += ":}";

			networkManager.client.StartSendThread(newConnectionData);

			initializedConnection = true; //CHANGING THIS VAR TO TRUE CAUSES THE ERROR
		}


		lastInitSend = startEng.timeManager.GetTime();
		inInitThread = false;
	}
}



void SendPosition(int playerNum_, float newX, float newY, float newZ) {

	std::lock_guard<std::mutex> guard(renderMutex);

	if (playerNum_ != clientPlayerNum) {
		Transform* referencedTransform = refHandler->playerNumToTransform[playerNum_];

		if (referencedTransform != nullptr) {
			referencedTransform->position = Vec3(newX, newY, newZ);
		}
	}

	if (networkManager.isServer) {

		positionData = "";
		positionData += "{SendPosition:";
		positionData += std::to_string(playerNum_);
		positionData += ":";
		positionData += std::to_string(newX);
		positionData += ":";
		positionData += std::to_string(newY);
		positionData += ":";
		positionData += std::to_string(newZ);
		positionData += ":}";

		networkManager.server.StartSendThread(positionData, true);
	}
}

void SendRotation(int playerNum_, float newX, float newY, float newZ) {

	std::lock_guard<std::mutex> guard(renderMutex);

	if (playerNum_ != clientPlayerNum) {
		Transform* referencedTransform = refHandler->playerNumToTransform[playerNum_];

		if (referencedTransform != nullptr) {
			referencedTransform->rotation = Vec3(newX, newY, newZ);
		}
	}

	if (networkManager.isServer) {

		rotationData = "";
		rotationData += "{SendRotation:";
		rotationData += std::to_string(playerNum_);
		rotationData += ":";
		rotationData += std::to_string(newX);
		rotationData += ":";
		rotationData += std::to_string(newY);
		rotationData += ":";
		rotationData += std::to_string(newZ);
		rotationData += ":}";

		networkManager.server.StartSendThread(rotationData, true);
	}
}






//Start of program
void ConnectionHandler::Start(Container* container) {
	networkManager.client.SetEndpoint("192.168.4.156", "8000"); //Server port


	networkManager.stringToVoid["SendPosition"] = SendPosition;
	networkManager.stringToVoid["SendRotation"] = SendRotation;
	networkManager.stringToVoid["AddConnection"] = AddConnection;
	networkManager.stringToVoid["RequestPlayerNum"] = RequestPlayerNum;
	networkManager.stringToVoid["SendPlayerNum"] = SendPlayerNum;


	if (networkManager.isServer) {
		networkManager.client.CloseClientSocket();
		networkManager.server.OpenServerSocket();
		networkManager.server.StartReceiveThread();
	}
	else {
		//wasServer = false;
		//networkManager.server.CloseServerSocket();
		networkManager.client.StartReceiveThread();
	}
	networkManager.StartServiceThread();

	connectionLoopFunctions.push_back(&SendLocalPosition);
	connectionLoopFunctions.push_back(&SendLocalRotation); //MODIFICATION ERROR: THIS CHANGES THE LOCAL CLIENTS ROTATION
	//Change how it's parsed and instead of creating multiple send threads per frame, do one that has all the data needed in that frame, like "{SendPosition:1:5.5:2.6:7.3:}{SendRotation:1:0.0:60.5:0.0:}{Animation:"Run"}"

	refHandler = this;
}

//Every frame
void ConnectionHandler::Update(float deltaTime, Container* container) {

	if (playerContainer == nullptr) {

		for (int i = 0; i < startEng.containers.size(); i++) {
			if (startEng.containers[i]->name == "Player") {
				playerContainer = startEng.containers[i];
				playerNumToTransform.emplace(1, &playerContainer->transform);
				playerTransform = &playerContainer->transform;
			}
		}
	}

	{
		std::lock_guard<std::mutex> guard(networkManager.receiveMutex);

		if (input.GetKeyState('R') && !networkManager.isServer) {
			networkManager.client.SetEndpoint("127.0.0.1", "8000"); //Server port
			networkManager.client.ConnectToServer();
		}

		if (input.GetKeyState('N') && networkManager.isServer) {
			networkManager.client.SetEndpoint("127.0.0.1", "8000"); //Server port
			networkManager.client.ConnectToServer();
			networkManager.isServer = false;
			networkManager.isClient = true;
			networkManager.server.CloseServerSocket();
			networkManager.client.OpenClientSocket();
			networkManager.client.StartReceiveThread();
		}
		if (input.GetKeyState('M') && !networkManager.isServer) {
			networkManager.isServer = true;
			networkManager.isClient = false;
			networkManager.client.CloseClientSocket();
			networkManager.server.OpenServerSocket();
			networkManager.server.SetPort(8000);
			networkManager.server.StartReceiveThread();
			//clientPlayerNum = 2;
		}

		{
			std::lock_guard<std::mutex> guard(inInitDelayMutex);

			//NOTE: This runs even on the server instance before it becomes a server and the user presses the 'M' key
			if (!initializedConnection && !networkManager.isServer && !inInitThread) {
				std::thread initThread(&ConnectionHandler::DelayInitializationSend, this);

				initThread.detach();
			}
		}

		//if connected
		{
			std::lock_guard<std::mutex> guard(inDelayMutex);
			std::lock_guard<std::mutex> initGuard(inInitDelayMutex);

			if (!networkManager.isServer && !inPositionThread && initializedConnection) {
				std::thread positionThread(&ConnectionHandler::DelayFrameSend, this);

				positionThread.detach();
			}
		}
	}
}