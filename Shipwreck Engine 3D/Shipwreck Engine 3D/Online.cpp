#include "Online.h"

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




void SendMessageToAddress(Connection connection, char message[bufferSize], SOCKET* socket) {
	//for (int i = 7777; i <= 65565; i++) {
		//connection.ipAddress.sin_port = htons(i); //port
		if (sendto(*socket, message, bufferSize, 0, (SOCKADDR*)&connection.ipAddress, sizeof(connection.ipAddress)) != SOCKET_ERROR) {
			return;
		}
	//}
}

void NetworkManager::AssignSendAddress(Connection* connection, std::string newIP) {
	for (int i = startPort; i <= endPort; i++) {
		if (i != thisAddress.port) {
			connection->ipAddress.sin_port = htons(i);
			connection->port = i;
			inet_pton(AF_INET, (newIP.c_str()/*targetIp.c_str()*/), &connection->ipAddress.sin_addr.S_un.S_addr); //ipAddress = (SOCKADDR*)connection.substr(0, i).c_str();
			return;
		}
	}
}


void RecieveMessages(NetworkManager networkManager) {
	char localBuffer[bufferSize];

	while (true) {
		int receiveSocketSize = sizeof(networkManager.receivingAddress.ipAddress);
		int receivedData = recvfrom(networkManager.sock, localBuffer, bufferSize, 0, (SOCKADDR*)&networkManager.receivingAddress.ipAddress, &receiveSocketSize);

		if (receivedData != SOCKET_ERROR) {
			//std::cout << "\nReceived Message: " << recBuffer << "\nSend Message: ";
			std::string bufferString = localBuffer;

			MessageInfo info = networkManager.BufferToInfo(bufferString);

			networkManager.HandleMessage(info);
		}
		else {
			//std::cout << "Socket receive error: " << WSAGetLastError();
		}
	}
}