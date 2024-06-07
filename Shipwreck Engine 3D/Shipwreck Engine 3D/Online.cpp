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


Server::Server(boost::asio::io_service& io_service, int port)
	: socket_(io_service, udp::endpoint(udp::v4(), 0)) {

	if (!networkManager.isServer) {
        boost::system::error_code ec;
		socket_.close(ec);

	}
    else {
        SetPort(port);
    }
}


void Server::OpenServerSocket() {
    boost::system::error_code ec;

    socket_.open(udp::v4(), ec);
}

void Server::CloseServerSocket() {
    boost::system::error_code ec;

    socket_.close(ec);
}


void NetworkManager::IOServiceThread() {

    io_service.run();
}



Client::Client(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
    : socket_(io_service, udp::endpoint(udp::v4(), 0)), resolver_(io_service) {

    udp::resolver::query query(udp::v4(), host, port);
    server_endpoint_ = *resolver_.resolve(query);

    boost::system::error_code ec;

    //socket_.open(udp::v4());

    //for (int i = startPort; i < endPort; i++) {
    //    socket_.bind(udp::endpoint(udp::v4(), i), ec);

    //    if (!ec) {
    //        //socket_ = udp::socket(io_service, udp::endpoint(udp::v4(), i));
    //        
    //        socket_.connect(server_endpoint_, ec);

    //        if (!ec) {

    //        }

    //        return;
    //    }
    //}
    //socket_.connect(server_endpoint_, ec);

}


void Server::SendData(std::string data, bool echoToAll) {
    if (echoToAll) {

        for (int i = 0; i < networkManager.allClients.size(); i++) {
            socket_.async_send_to(
                boost::asio::buffer(data), networkManager.allClients[i],
                [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
                });
        }
    }
    else {
        //std::lock_guard<std::mutex> receiveGuard(networkManager.receiveMutex);

        socket_.async_send_to(
            boost::asio::buffer(data), networkManager.allClients.back(),
            [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            });
    }
}


void Server::ReceiveData() {
    while (true) {
        {
            std::lock_guard<std::mutex> guard(networkManager.receiveMutex);
            if (!networkManager.isServer) {
                break;
            }
        }

        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                    
                    bool listHasClient = false;
                    for (int i = 0; i < networkManager.allClients.size(); i++) {
                        if (networkManager.allClients[i].address() == remote_endpoint_.address() && networkManager.allClients[i].port() == remote_endpoint_.port()) {
                            listHasClient = true;
                            break;
                        }
                    }
                    if (!listHasClient) {
                        networkManager.allClients.push_back(remote_endpoint_);
                    }
                    
                    HandleMessage(recv_buffer_.data(), bytes_recvd);
                    
                    //echoes to client again

                    recv_buffer_.fill(0);
                }
            });
    }
}


void Client::ReceiveData() {
    while (true) {
        {
            std::lock_guard<std::mutex> guard(networkManager.receiveMutex);
            if (networkManager.isServer) {
                break;
            }
        }

        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), sender_endpoint_,
            [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {

                    HandleMessage(recv_buffer_.data(), bytes_recvd);
                    recv_buffer_.fill(0);
                }
            });
    }
}



void Server::HandleMessage(const char* data, std::size_t length) {
    BufferInfo newBufferInfo = networkManager.BufferToBufferInfo(data);

    for (int i = 0; i < newBufferInfo.messageInfoList.size(); i++) {

        networkManager.HandleMessage(newBufferInfo.messageInfoList[i]);
    }
}


void Client::HandleMessage(const char* data, std::size_t length) {

    BufferInfo newBufferInfo = networkManager.BufferToBufferInfo(data);

    for (int i = 0; i < newBufferInfo.messageInfoList.size(); i++) {

        networkManager.HandleMessage(newBufferInfo.messageInfoList[i]);
    }
}


void Server::SetPort(int newPort) {
    
    socket_ = udp::socket(networkManager.io_service, udp::endpoint(udp::v4(), newPort));
}



void NetworkManager::HandleMessage(MessageInfo info) {
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
    else if (info.type == "NumberedFloat3") {
        NumberedFloat3FunctionCall func = reinterpret_cast<NumberedFloat3FunctionCall>(functionCall);
        func(stoi(info.calls[1]), stof(info.calls[2]), stof(info.calls[3]), stof(info.calls[4]));
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