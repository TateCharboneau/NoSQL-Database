#include <iostream>
#include "RedisServer.hpp"
#include "ServerSocket.hpp"

int main() {
    RedisServer server;
    ServerSocket socketServer;

    if (!socketServer.start(8080)) {
        std::cerr << "Failed to start the server." << std::endl;
        return 1;
    }

    std::cout << "Server is running on port 8080..." << std::endl;
    socketServer.run(server);

    return 0;
}