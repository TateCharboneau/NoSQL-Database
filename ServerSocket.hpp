#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <winsock2.h> // socket handling
#include <ws2tcpip.h> // socket handling
#include <string>
#include "RedisServer.hpp"

using namespace std;

class ServerSocket {
public:
    ServerSocket();
    ~ServerSocket();
    bool start(int port);
    void run(RedisServer& server);

private:
    SOCKET serverSocket;
    bool initWinsock();
    bool handleClient(SOCKET clientSocket, RedisServer& server);
};

#endif