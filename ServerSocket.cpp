#include "ServerSocket.hpp"
#include <iostream>

ServerSocket::ServerSocket() : serverSocket(INVALID_SOCKET) {}


// destructor w/ cleanup
ServerSocket::~ServerSocket() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
    }
    WSACleanup();
}

// initialize winsock, return true if successful
bool ServerSocket::initWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

// start server on port
bool ServerSocket::start(int port) {
    if (!initWinsock()) {
        cerr << "WSAStartup failed." << endl;
        return false;
    }

    // socket creation
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        return false;
    }

    // socket binding
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed." << endl;
        closesocket(serverSocket);
        return false;
    }

    // socket listening
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed." << endl;
        closesocket(serverSocket);
        return false;
    }

    return true;
}

// accept and handle client communication
void ServerSocket::run(RedisServer& server) {
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed." << endl;
            continue;
        }

        handleClient(clientSocket, server);
        closesocket(clientSocket);
    }
}

//handle client communication
void ServerSocket::handleClient(SOCKET clientSocket, RedisServer& server) {
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        string userInput(buffer);

        if (userInput.substr(0, 3) == "SET") {
            size_t spacePos = userInput.find(' ', 4);
            string key = userInput.substr(4, spacePos - 4);
            string value = userInput.substr(spacePos + 1);

            server.set(key, value);
            send(clientSocket, "OK", 2, 0);
        }
        
        else if (userInput.substr(0, 3) == "GET") {
            string key = userInput.substr(4);
            string value = server.get(key);

            if (value.empty()) {
                send(clientSocket, "ERROR: No key found", 19, 0);
            }
            else {
                send(clientSocket, value.c_str(), value.size(), 0);
            }

        }
        
        else if (userInput.substr(0, 3) == "DEL") {
            string key = userInput.substr(4);
            bool successful = server.del(key);

            if (successful) {
                send(clientSocket, "OK", 2, 0);
            }
            else {
                send(clientSocket, "ERROR: No key found", 19, 0);
            }

        }
        
        else {
            send(clientSocket, "ERROR: Unknown command", 22, 0);
        }
    }
}