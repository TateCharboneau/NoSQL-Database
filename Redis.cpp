#include <iostream>
#include <unordered_map>
#include <string>
#include <winsock2.h> // Socket Handling
#include <ws2tcpip.h> // Socket Handling
using namespace std;

class RedisServer {

    public:

        void set(const string& key, const string& value) {
            store[key] = value;
        }

        string get(const string& key) const {
            auto cur = store.find(key);
            if (cur != store.end()) { // checks if cur = end of store
                return cur->second; // first stores key and second stores data value
            }
            return "";
        }

        // Delete a key-value pair
        bool del(const string& key) {
            return store.erase(key) != 0; // store.erase(key): Returns 1 if key found, 0 if not found
        }


    private:

        unordered_map<string, string> store;
};

int main() {
    RedisServer server;
    server.set("name", "RedisRec");
    cout << "Get 'name': " << server.get("name") << endl;
    server.del("name");
    cout << "Get 'name' after delete: " << server.get("name") << endl;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1; // Handle error
    }

    // Step 2: Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1; // Handle error
    }

    // Step 3: Bind the socket
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
    serverAddr.sin_port = htons(8080); // In port

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1; // Handle error
    }

    // Step 4: Listen for connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1; // Handle error
    }

    // Step 5: Accept connections in a loop
    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed." << std::endl;
            continue; // Handle error, but keep running the server
        }

        // Step 6: Handle client communication (receive and send data)
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0'; // Null-terminate the received data
            std::cout << "Received: " << buffer << std::endl;

            // Echo back the received data
            send(clientSocket, buffer, bytesReceived, 0);
        }

        closesocket(clientSocket); // Close the client socket
    }

    // Step 7: Clean up
    closesocket(serverSocket);
    WSACleanup();


    return 0;
}