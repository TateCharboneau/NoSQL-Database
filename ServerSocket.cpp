#include "ServerSocket.hpp"


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

    bool serverRunning = true;

    while (serverRunning) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed." << endl;
            continue;
        }

        if (!handleClient(clientSocket, server)) {
            serverRunning = false;
        }

        closesocket(clientSocket);
    }
}

//handle client communication
bool ServerSocket::handleClient(SOCKET clientSocket, RedisServer& server) {
    char buffer[1024];
    string userInput = "";

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  // null-terminate
            userInput.append(buffer);  // add the received data to userInput string

            // check if newline is present e.g. command complete
            size_t pos = userInput.find('\n');
            if (pos != string::npos) {
                userInput.erase(userInput.find_last_not_of("\r\n") + 1);  // remove trailing whitespace
                cout << "Received command: " << userInput << endl;

                // Process the command
                if (userInput.substr(0, 3) == "SET") {
                    size_t spacePos = userInput.find(' ', 4);
                    string key = userInput.substr(4, spacePos - 4);
                    string value = userInput.substr(spacePos + 1);
                    server.set(key, value);
                    server.saveToFile(key, value);
                    send(clientSocket, "OK", 2, 0);
                }
                else if (userInput.substr(0, 3) == "GET") {
                    string key = userInput.substr(4);
                    string value = server.get(key);
                    if (value.empty()) {
                        send(clientSocket, "ERROR: No key found\r\n", 19, 0);
                    } else {
                        string response = value + "\n";
                        send(clientSocket, response.c_str(), response.size(), 0);
                    }
                }
                else if (userInput.substr(0, 3) == "DEL") {
                    string key = userInput.substr(4);
                    bool successful = server.del(key);
                    if (successful) {
                        send(clientSocket, "OK\n", 2, 0);
                    } else {
                        send(clientSocket, "ERROR: No key found\r\n", 19, 0);
                    }
                }
                else if (userInput == "QUIT") {
                    send(clientSocket, "Closing connection. Goodbye!\r\n", 29, 0);
                    cout << "Client requested to quit. Closing connection." << endl;
                    break;
                }
                else {
                    send(clientSocket, "ERROR: Unknown command\r\n", 22, 0);
                }
                
                userInput.clear(); // clear the userInput buffer for the next command
            }
        } 
        else if (bytesReceived == 0) {
            // connection closed gracefully by the client
            cout << "Client disconnected.\r\n" << endl;
            break;
        } 
        else {
            // error receiving data
            cerr << "Error receiving data from client.\r\n" << endl;
            break;
        }
    }

    return false;
    closesocket(clientSocket);
}