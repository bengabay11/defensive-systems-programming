#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock";
        return;
    }

    // Create a socket
    this->socketObject = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketObject == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError();
        WSACleanup();
        return;
    }
}

void ClientSocket::connectToServer(std::string host, int port)
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    InetPton(AF_INET, std::wstring(host.begin(), host.end()).c_str(), &serverAddress.sin_addr.s_addr);

    // Connect to the server
    int iResult = connect(this->socketObject, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (iResult < 0) {
        std::cerr << "Failed to connect to the server";
        this->close();
    }
}

void ClientSocket::sendData(char* buffer, long bufferSize)
{
    if (send(this->socketObject, buffer, bufferSize, 0) < 0) {
        std::cerr << "Failed to send data to the server";
        this->close();
    }
}

void ClientSocket::receive(char* buffer, long bufferSize)
{
    int bytesRead = recv(this->socketObject, buffer, bufferSize, 0);
    if (bytesRead < 0) {
        std::cerr << "Failed to receive data from the server";
        this->close();
    }
}

void ClientSocket::close()
{
    closesocket(this->socketObject);
    WSACleanup();
    exit(1);
}
