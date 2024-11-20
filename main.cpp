#include <iostream>
#include "RedisServer.hpp"
#include "ServerSocket.hpp"


int main() {
    RedisServer server;
    ServerSocket socketServer;

    server.loadFromFile();

    if (!socketServer.start(8080)) {
        cerr << "Failed to start the server." << endl;
        return 1;
    }

    cout << "Server is running on port 8080..." << endl;
    socketServer.run(server);

    return 0;
}