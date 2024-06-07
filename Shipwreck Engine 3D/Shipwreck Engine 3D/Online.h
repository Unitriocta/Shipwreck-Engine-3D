#pragma once


//#include <winsock2.h> //socket manager for sending packets to other devices
//#include <ws2tcpip.h> //socket manager for sending packets to other devices
#include <windows.h>

//#pragma comment(lib, "ws2_32.lib")

#include "MathExtras.h"
#include "Transform.h"


#include <string>
#include <vector>

#include <type_traits>

#include <thread>
#include <mutex>

#include <iostream>
#include <map>

#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;


class NetworkManager;

class Server;

class Client;

const int bufferSize = 1024;




//class Connection {
//public:
//    Connection() {
//
//    }
//
//    void SetEndpoint(std::string ipAddress, int port_) {
//
//        endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ipAddress), port_);
//        port = port_;
//    }
//
//    boost::asio::ip::udp::endpoint endpoint;
//    int port;
//};


struct MessageInfo {
public:
    MessageInfo() {
    }

    std::vector<std::string> calls;
    std::string type;
};


struct BufferInfo {
public:
    BufferInfo() {
    }

    std::vector<MessageInfo> messageInfoList;
};




void PrintHi();
void PrintString(std::string in);
void PrintInt(int in);
void PrintFloat(float in);
void SetPlayerPosition(Vec3 in, int inPlayerNum, NetworkManager* networkManager);


class Server;
class Client;


class Server {
public:
    Server(boost::asio::io_service& io_service, int port);

public:

    void OpenServerSocket();
    void CloseServerSocket();

    void SetPort(int newPort);

    void SendData(std::string data, bool echoToAll);

    void ReceiveData();

    void StartReceiveThread() {
        std::thread receiveThread(&Server::ReceiveData, this);
        receiveThread.detach();
    }

    void StartSendThread(std::string data, bool echoToAll) {
        std::thread sendThread(&Server::SendData, this, data, echoToAll);
        sendThread.detach();
    }

    void HandleMessage(const char* data, std::size_t length);

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, bufferSize> recv_buffer_;
};

class Client {
public:
    Client(boost::asio::io_service& io_service, const std::string& host, const std::string& port);

    
    int startPort = 7777;
    int endPort = 65535;

public:

    void OpenClientSocket() {
        socket_.open(udp::v4());
    }
    void CloseClientSocket() {
        socket_.close();
    }

    void SendData(std::string data) {
        socket_.async_send_to(
            boost::asio::buffer(data), server_endpoint_,
            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            });
    }


    bool ConnectToServer() {

        boost::system::error_code ec;

        for (int i = 0; i < 4; i++) {

            socket_.connect(server_endpoint_, ec);

            if (!ec) {
                return true;
            }
        }

        return false;
    }


    void ReceiveData();

    void StartReceiveThread() {
        std::thread receiveThread(&Client::ReceiveData, this);
        receiveThread.detach();
    }

    void StartSendThread(std::string data) {
        std::thread sendThread(&Client::SendData, this, data);
        sendThread.detach();
    }

    void HandleMessage(const char* data, std::size_t length);


    void SetEndpoint(const std::string& endIP, const std::string& port) {

        udp::resolver::query query(udp::v4(), endIP, port);
        server_endpoint_ = *resolver_.resolve(query);
    }


    udp::socket socket_;
    udp::endpoint server_endpoint_;
    udp::endpoint sender_endpoint_;
    udp::resolver resolver_;
    boost::array<char, bufferSize> recv_buffer_;
};

class NetworkManager {

public:
    NetworkManager() :
        work(io_service), server(io_service, 8000), client(io_service, "127.0.0.1", "8000")
    {

    }

public:
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

    void StartServiceThread() {
        std::thread serviceThread(&NetworkManager::IOServiceThread, this);
        serviceThread.detach();
    }


    void IOServiceThread();


    BufferInfo BufferToBufferInfo(std::string buffer) {

        bool inQuote = false;
        int messageStartChar = 0;
        BufferInfo bufferInfo;

        std::string curMessage;

        for (int i = 0; i < buffer.length(); i++) {
            if (buffer[i] == '{' && !inQuote) {
                messageStartChar = i + 1;
            }
            else if (buffer[i] == '}' && !inQuote) {
                curMessage = buffer.substr(messageStartChar, i - messageStartChar);
                bufferInfo.messageInfoList.push_back(BufferToInfo(curMessage));
            }

            if (buffer[i] == '\"' || buffer[i] == '\'') {

                inQuote = !inQuote;
            }
        }

        return bufferInfo;
    }


    MessageInfo BufferToInfo(std::string buffer) {

        bool inQuote = false;
        int prevCallChar = 0;
        MessageInfo messageInfo;

        int callsTotal = 0;

        std::vector<bool> areStrings;
        std::vector<bool> areInts;
        std::vector<bool> areFloats;

        for (int i = 0; i < buffer.length(); i++) {

            if (buffer[i] == ':' && !inQuote) {
                messageInfo.calls.push_back(buffer.substr(prevCallChar, i - prevCallChar));

                if (callsTotal == 0) {

                    areStrings.push_back(false);
                    areInts.push_back(false);
                    areFloats.push_back(false);
                }

                if (callsTotal >= areStrings.size()) {

                    areStrings.push_back(false);
                    areInts.push_back(true);
                    areFloats.push_back(false);
                }

                callsTotal++;
                prevCallChar = i + 1;
            }
            else if (buffer[i] == '.' && !inQuote) {

                if (areStrings.size() <= callsTotal) {

                    areStrings.push_back(false);
                    areInts.push_back(false);
                    areFloats.push_back(true);
                }
            }
            else if (buffer[i] == '\"' || buffer[i] == '\'') {

                if (areStrings.size() <= callsTotal) {

                    areStrings.push_back(true);
                    areInts.push_back(false);
                    areFloats.push_back(false);
                }

                inQuote = !inQuote;
            }
        }

        if (messageInfo.calls.size() == 1) {

            messageInfo.type = "NoInput";
        }
        else if (messageInfo.calls.size() == 2) {

            if (areStrings[1]) {
                messageInfo.type = "String";
            }
            else if (areFloats[1]) {
                messageInfo.type = "Float";
            }
            else if (areInts[1]) {
                messageInfo.type = "Int";
            }
        }
        else if (messageInfo.calls.size() == 3) {

            if (areFloats[1] && areFloats[2]) {
                messageInfo.type = "Float2";
            }
            else if (areInts[1] && areInts[2]) {
                messageInfo.type = "Int2";
            }


            else if (areStrings[1] && areStrings[2]) {
                messageInfo.type = "NamedString";
            }
            else if (areStrings[1] && areFloats[2]) {
                messageInfo.type = "NamedFloat";
            }
            else if (areStrings[1] && areInts[2]) {
                messageInfo.type = "NamedInt";
            }
        }
        else if (messageInfo.calls.size() == 4) {

            if (areFloats[1] && areFloats[2] && areFloats[3]) {
                messageInfo.type = "Float3";
            }
            else if (areInts[1] && areInts[2] && areInts[3]) {
                messageInfo.type = "Int3";
            }


            else if (areStrings[1] && areFloats[2] && areFloats[3]) {
                messageInfo.type = "NamedFloat2";
            }
            else if (areStrings[1] && areInts[2] && areInts[3]) {
                messageInfo.type = "NamedInt2";
            }
        }
        else if (messageInfo.calls.size() == 5) {

            if (areStrings[1] && areFloats[2] && areFloats[3] && areFloats[4]) {
                messageInfo.type = "NamedFloat3";
            }
            else if (areStrings[1] && areInts[2] && areInts[3] && areInts[4]) {
                messageInfo.type = "NamedInt3";
            }
            else if (areInts[1] && areFloats[2] && areFloats[3] && areFloats[4]) {
                messageInfo.type = "NumberedFloat3";
            }
        }
        else if (messageInfo.calls.size() == 6) {
            messageInfo.type = "PlayerFloat3";
        }
        return messageInfo;
    }

    void HandleMessage(MessageInfo info);

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

public:
    typedef void (*NoInFunctionCall)();

    typedef void (*IntFunctionCall)(int);
    typedef void (*NamedIntFunctionCall)(std::string, int);
    typedef void (*Int2FunctionCall)(int, int);
    typedef void (*NamedInt2FunctionCall)(std::string, int, int);
    typedef void (*Int3FunctionCall)(int, int, int);
    typedef void (*NamedInt3FunctionCall)(std::string, int, int, int);

    typedef void (*FloatFunctionCall)(float);
    typedef void (*NamedFloatFunctionCall)(std::string, float);
    typedef void (*Float2FunctionCall)(float, float);
    typedef void (*NamedFloat2FunctionCall)(std::string, float, float);
    typedef void (*Float3FunctionCall)(float, float, float);
    typedef void (*NamedFloat3FunctionCall)(std::string, float, float, float);
    typedef void (*NumberedFloat3FunctionCall)(int, float, float, float);

    typedef void (*StringFunctionCall)(std::string);
    typedef void (*NamedStringFunctionCall)(std::string, std::string);


    typedef void (*PlayerFloat3FunctionCall)(Vec3, int, NetworkManager*);


    std::map<std::string, void*> stringToVoid;


    std::map<int, Transform*> transformAddresses;


    std::vector<udp::endpoint> allClients;

    char receiveBuffer_[bufferSize];


    boost::asio::io_service io_service;

    boost::asio::io_service::work work;

    Server server;
    Client client;

    bool isClient = true;
    bool isServer = false;

    std::mutex receiveMutex;
    std::mutex ioMutex;
};



