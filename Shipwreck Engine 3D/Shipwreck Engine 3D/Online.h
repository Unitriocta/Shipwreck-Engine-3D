#pragma once


#include <winsock2.h> //socket manager for sending packets to other devices
#include <ws2tcpip.h> //socket manager for sending packets to other devices
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#include "MathExtras.h"
#include "Transform.h"

#include <thread>

#include <map>


#include <iostream>
#include <string>
#include <vector>

#include <type_traits>

#include <thread>

#include <map>


const int bufferSize = 100;




class Connection {
public:
	Connection() {

	}
	bool SeparateFullAddress(std::string connection) {
		for (int i = 0; i < connection.length(); i++) {
			if (connection[i] == ':') {
				inet_pton(AF_INET, (connection.c_str()/*targetIp.c_str()*/), &ipAddress.sin_addr.S_un.S_addr); //ipAddress = (SOCKADDR*)connection.substr(0, i).c_str();
				port = std::stoi(connection.substr(i + 1, connection.length() - i));
				return true;
			}
		}

		return false;
	}

	SOCKADDR_IN ipAddress;
	int port;
};


struct MessageInfo {
public:
	MessageInfo() {
		isString = false;
		isFloat = false;
	}

	std::vector<std::string> calls;
	bool isString;
	bool isFloat;
	bool isFloat2;
	bool isFloat3;
	bool isPlayerVector3;
};

class NetworkManager;

void PrintHi();
void PrintInt(int in);
void PrintFloat(float in);
void PrintString(std::string in);

void SetPosition(Vec3 in);
void SetPlayerPosition(Vec3 in, int inPlayerNum, NetworkManager* networkManager);

void SendMessageToAddress(Connection connection, char message[bufferSize], SOCKET* socket);


void RecieveMessages(NetworkManager networkManager);



class NetworkManager
{
public:
	NetworkManager() {
		if (WSAStartup(sockVersion, &sock_data)) {
			//std::cout << "Setup error: " << WSAGetLastError();
		}


		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		//Address Fam^, IPV4
		//UDP socket is SOCK_DGRAM
		//Protocol is UDP so IPPROTO_UDP
		if (sock == INVALID_SOCKET) {
			//std::cout << "Socket error: " << WSAGetLastError();
		}


		thisAddress.ipAddress.sin_family = AF_INET; //UDP socket
		thisAddress.ipAddress.sin_addr.s_addr = INADDR_ANY; //s_addr is IPV4
		for (int i = startPort; i <= endPort; i++) {
			thisAddress.ipAddress.sin_port = htons(i); //port
			if (::bind(sock, (SOCKADDR*)&thisAddress.ipAddress, sizeof(thisAddress.ipAddress)) != SOCKET_ERROR) {
				thisAddress.port = i;
			}
		}
	}

	




	void AssignSendAddress(Connection* connection, std::string newIP);

	

	void StartMessageThread(char message[bufferSize], int playerNum) {
		std::thread messageThread(SendMessageToAddress, sendingAddresses[playerNum], message, &sock);
		messageThread.detach();
	}

	void StartRecieveThread(NetworkManager networkManager) {
		std::thread messageThread(RecieveMessages, networkManager);
		messageThread.detach();
	}


	void HandleMessage(MessageInfo info) {
		void* functionCall = MessageToFunctionCall(info);

		if (functionCall == nullptr) {
			return;
		}

		if (info.calls.size() >= 2) {
			if (info.calls[1] == "t") {

				return;
			}
		}

		if (info.calls.size() == 2) {
			if (info.isString) {
				info.calls[1] = RemoveQuotes(info.calls[1]);

				StringInFunctionCall func = reinterpret_cast<StringInFunctionCall>(functionCall);
				func(info.calls[1]);
			}
			else if(info.isFloat) {
				FloatInFunctionCall func = reinterpret_cast<FloatInFunctionCall>(functionCall);
				func(stof(info.calls[1]));
			}
			else {
				IntInFunctionCall func = reinterpret_cast<IntInFunctionCall>(functionCall);
				func(stoi(info.calls[1]));
			}
		}
		else if (info.calls.size() == 4) {
			if (info.isFloat3) {
				Float3InFunctionCall func = reinterpret_cast<Float3InFunctionCall>(functionCall);
				func(Vec3(stof(info.calls[1]), stof(info.calls[2]), stof(info.calls[3])));
			}
		}
		else if (info.calls.size() == 5) {
			if (info.isPlayerVector3) {
				PlayerFloat3FunctionCall func = reinterpret_cast<PlayerFloat3FunctionCall>(functionCall);
				func(Vec3(stof(info.calls[1]), stof(info.calls[2]), stof(info.calls[3])), stoi(info.calls[4]), this);
			}
		}
		else if (info.calls.size() == 1) {
			NoInFunctionCall func = reinterpret_cast<NoInFunctionCall>(functionCall);
			func();
		}
	}



	void* MessageToFunctionCall(MessageInfo info) {
		if (info.calls.size() >= 1) {
			return stringToVoid[info.calls[0]];

		}
		return nullptr;
	}

	std::string FunctionCallToMessage(void* functionCall) {
		for (const auto& pair : stringToVoid) {
			if (pair.second == functionCall) {
				return pair.first;
			}
		}
	}




	bool IsString(std::string stringIn) {
		if (stringIn.find("\"") || stringIn.find("\'")) {
			return true;
		}
		return false;
	}

	std::string RemoveQuotes(std::string inString) {
		int strLen = inString.length();
		if (strLen >= 3) {
			return inString.substr(1, inString.length() - 2);
		}
		else {
			return inString;
		}
	}




	MessageInfo BufferToInfo(std::string buffer) {

		bool inQuote = false;
		int prevCallChar = 0;
		MessageInfo messageInfo;

		int callsTotal = 0;

		for (int i = 0; i < buffer.length(); i++) {

			if (buffer[i] == ':' && !inQuote) {
				//new call
				messageInfo.calls.push_back(buffer.substr(prevCallChar, i - prevCallChar));

				callsTotal++;
				prevCallChar = i + 1;
				//return buffer.substr(0, i);
			}
			else if (buffer[i] == '.' && !inQuote && callsTotal == 1) {
				messageInfo.isFloat = true;
			}
			else if (buffer[i] == '.' && !inQuote && callsTotal == 2) {
				messageInfo.isFloat = true;
				messageInfo.isFloat2 = true;
			}
			else if (buffer[i] == '.' && !inQuote && callsTotal == 3) {
				messageInfo.isFloat = false;
				messageInfo.isFloat2 = false;
				messageInfo.isFloat3 = true;
			}
			else if (buffer[i] != '\'' && buffer[i] != '\"' && !inQuote && callsTotal == 4) {
				messageInfo.isFloat = false;
				messageInfo.isFloat2 = false;
				messageInfo.isFloat3 = false;
				messageInfo.isPlayerVector3 = true;
			}
			else if (buffer[i] == '\"') {
				messageInfo.isString = true;
				inQuote = !inQuote;
			}
			else if (buffer[i] == '\'') {
				messageInfo.isString = true;
				inQuote = !inQuote;
			}
		}

		return messageInfo;
	}


	std::string DomainToIP(const std::string& domainName) {

		struct addrinfo hints = {};
		hints.ai_family = AF_INET; // AF_INET for IPv4
		hints.ai_socktype = SOCK_STREAM;

		struct addrinfo* res;
		int result = getaddrinfo(domainName.c_str(), "http", &hints, &res);
		if (result != 0) {
			std::cerr << "getaddrinfo failed with error: " << result << std::endl;
			return "";
		}

		char ipStr[INET_ADDRSTRLEN];
		for (struct addrinfo* p = res; p != nullptr; p = p->ai_next) {
			inet_ntop(p->ai_family, &((struct sockaddr_in*)p->ai_addr)->sin_addr, ipStr, sizeof(ipStr));
		}

		freeaddrinfo(res);

		return std::string(ipStr);
	}

public:


	typedef void (*NoInFunctionCall)();
	typedef void (*IntInFunctionCall)(int);
	typedef void (*FloatInFunctionCall)(float);
	typedef void (*Float3InFunctionCall)(Vec3);
	typedef void (*PlayerFloat3FunctionCall)(Vec3,int, NetworkManager*);
	typedef void (*StringInFunctionCall)(std::string);

	typedef void (NetworkManager::* Action)();

	std::map<std::string, void*> stringToVoid = {
	{"PrintHi",      PrintHi},
	{"PrintString",  PrintString},
	{"PrintInt",     PrintInt},
	{"PrintFloat",   PrintFloat},
	{"SetPosition",  SetPosition},
	{"SetPlayerPosition",  SetPlayerPosition}
	};

	int startPort = 7777;
	int endPort = 65535;

	std::string targetIp;
	WORD sockVersion = 0x202;
	WSAData sock_data;

	SOCKET sock;

	Connection thisAddress; //local IP(V4) address of this computer
	std::vector<Connection> sendingAddresses;

	std::map<int, Transform*> transformAddresses;

	Connection receivingAddress; //IP(V4) address of computer recieving from
};