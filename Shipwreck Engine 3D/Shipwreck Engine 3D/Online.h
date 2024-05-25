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
//#include <mutex>

#include <iostream>
#include <map>

#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::udp;


class NetworkManager;

class Server;

class Client;

const int bufferSize = 100;




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





void PrintHi();
void PrintString(std::string in);
void PrintInt(int in);
void PrintFloat(float in);
void SetPlayerPosition(Vec3 in, int inPlayerNum, NetworkManager* networkManager);


class Server;
class Client;


class Server {
public:
    Server(boost::asio::io_service& io_service, int port)
        : socket_(io_service, udp::endpoint(udp::v4(), port)) {
        //ReceiveData();
    }

public:
    void SendData(std::string data) {
        socket_.async_send_to(
            boost::asio::buffer(data), remote_endpoint_,
            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            });
    }

    void ReceiveData() {
        while (true) {
            socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), remote_endpoint_,
                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                    if (!ec && bytes_recvd > 0) {
                        HandleMessage(recv_buffer_.data(), bytes_recvd);

                        //echoes to client again
                        socket_.async_send_to(
                            boost::asio::buffer(recv_buffer_, bytes_recvd), remote_endpoint_,
                            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
                            });
                    }
                });
        }
    }

    void StartReceiveThread() {
        std::thread receiveThread(&Server::ReceiveData, this);
        receiveThread.detach();
    }

    void StartSendThread(std::string data) {
        std::thread sendThread(&Server::SendData, this, data);
        sendThread.detach();
    }

    void HandleMessage(const char* data, std::size_t length);

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, bufferSize> recv_buffer_;
};

class Client {
public:
    Client(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
        : socket_(io_service, udp::endpoint(udp::v4(), 0)), resolver_(io_service) {
        udp::resolver::query query(udp::v4(), host, port);
        server_endpoint_ = *resolver_.resolve(query);
    }

public:
    void SendData(std::string data) {
        socket_.async_send_to(
            boost::asio::buffer(data), server_endpoint_,
            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            });
    }



    void ReceiveData() {
        while (true) {
            socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), sender_endpoint_,
                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                    if (!ec && bytes_recvd > 0) {

                        HandleMessage(recv_buffer_.data(), bytes_recvd);
                    }
                });
        }
    }

    void StartReceiveThread() {
        std::thread receiveThread(&Client::ReceiveData, this);
        receiveThread.detach();
    }

    void StartSendThread(std::string data) {
        std::thread sendThread(&Client::SendData, this, data);
        sendThread.detach();
    }

    void HandleMessage(const char* data, std::size_t length);


    udp::socket socket_;
    udp::endpoint server_endpoint_;
    udp::endpoint sender_endpoint_;
    udp::resolver resolver_;
    boost::array<char, bufferSize> recv_buffer_;
};

class NetworkManager {

public:
    NetworkManager() :
        work(io_service), server(io_service, 7777), client(io_service, "127.0.0.1", "7777")
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
        }
        else if (messageInfo.calls.size() == 6) {
            messageInfo.type = "PlayerFloat3";
        }

        return messageInfo;
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

        if (info.type == "NoInput") {
            NoInFunctionCall func = reinterpret_cast<NoInFunctionCall>(functionCall);
            func();
        }
        else if (info.type == "String") {
            info.calls[1] = RemoveQuotes(info.calls[1]);

            StringFunctionCall func = reinterpret_cast<StringFunctionCall>(functionCall);
            func(info.calls[1]);
        }
        else if (info.type == "NamedString") {
            info.calls[1] = RemoveQuotes(info.calls[1]);
            info.calls[2] = RemoveQuotes(info.calls[2]);

            NamedStringFunctionCall func = reinterpret_cast<NamedStringFunctionCall>(functionCall);
            func(info.calls[1], info.calls[2]);
        }
        else if (info.type == "Float") {
            FloatFunctionCall func = reinterpret_cast<FloatFunctionCall>(functionCall);
            func(stof(info.calls[1]));
        }
        else if (info.type == "NamedFloat") {
            info.calls[1] = RemoveQuotes(info.calls[1]);

            NamedFloatFunctionCall func = reinterpret_cast<NamedFloatFunctionCall>(functionCall);
            func(info.calls[1], stof(info.calls[2]));
        }
        else if (info.type == "Float2") {
            Float2FunctionCall func = reinterpret_cast<Float2FunctionCall>(functionCall);
            func(stof(info.calls[1]), stof(info.calls[2]));
        }
        else if (info.type == "NamedFloat2") {
            info.calls[1] = RemoveQuotes(info.calls[1]);

            NamedFloat2FunctionCall func = reinterpret_cast<NamedFloat2FunctionCall>(functionCall);
            func(info.calls[1], stof(info.calls[2]), stof(info.calls[3]));
        }
        else if (info.type == "Float3") {
            Float3FunctionCall func = reinterpret_cast<Float3FunctionCall>(functionCall);
            func(stof(info.calls[1]), stof(info.calls[2]), stof(info.calls[3]));
        }
        else if (info.type == "NamedFloat3") {
            info.calls[1] = RemoveQuotes(info.calls[1]);

            NamedFloat3FunctionCall func = reinterpret_cast<NamedFloat3FunctionCall>(functionCall);
            func(info.calls[1], stof(info.calls[2]), stof(info.calls[3]), stof(info.calls[4]));
        }
        else if (info.type == "Int") {
            IntFunctionCall func = reinterpret_cast<IntFunctionCall>(functionCall);
            func(stoi(info.calls[1]));
        }
        else if (info.type == "NamedInt") {
            info.calls[1] = RemoveQuotes(info.calls[1]);

            NamedIntFunctionCall func = reinterpret_cast<NamedIntFunctionCall>(functionCall);
            func(info.calls[1], stoi(info.calls[2]));
        }
        else if (info.type == "Int2") {
            Int2FunctionCall func = reinterpret_cast<Int2FunctionCall>(functionCall);
            func(stoi(info.calls[1]), stoi(info.calls[2]));
        }
        else if (info.type == "NamedInt2") {
            info.calls[1] = RemoveQuotes(info.calls[1]);

            NamedInt2FunctionCall func = reinterpret_cast<NamedInt2FunctionCall>(functionCall);
            func(info.calls[1], stoi(info.calls[2]), stoi(info.calls[3]));
        }
        else if (info.type == "Int3") {
            Int3FunctionCall func = reinterpret_cast<Int3FunctionCall>(functionCall);
            func(stoi(info.calls[1]), stoi(info.calls[2]), stoi(info.calls[3]));
        }
        else if (info.type == "NamedInt3") {
            info.calls[1] = RemoveQuotes(info.calls[1]);

            NamedInt3FunctionCall func = reinterpret_cast<NamedInt3FunctionCall>(functionCall);
            func(info.calls[1], stoi(info.calls[2]), stoi(info.calls[3]), stoi(info.calls[4]));
        }
        else if (info.type == "PlayerFloat3") {

            PlayerFloat3FunctionCall func = reinterpret_cast<PlayerFloat3FunctionCall>(functionCall);
            func(Vec3(stof(info.calls[1]), stof(info.calls[2]), stof(info.calls[3])), stoi(info.calls[4]), this);
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

    typedef void (*StringFunctionCall)(std::string);
    typedef void (*NamedStringFunctionCall)(std::string, std::string);


    typedef void (*PlayerFloat3FunctionCall)(Vec3, int, NetworkManager*);


    std::map<std::string, void*> stringToVoid = {
            {"PrintHi",      PrintHi},
            {"PrintInt",      PrintInt},
            {"PrintFloat",      PrintFloat},
            {"PrintString",      PrintString},
            {"SetPlayerPosition", SetPlayerPosition}
    };


    std::map<int, Transform*> transformAddresses;


    char receiveBuffer_[bufferSize];

    int startPort = 7777;
    int endPort = 65535;


    boost::asio::io_service io_service;

    boost::asio::io_service::work work;

    Server server;
    Client client;

    bool isClient = true;
};



