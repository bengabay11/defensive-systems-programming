#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw InitWinsockException();
    }

    this->socketObject = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketObject == INVALID_SOCKET) {
        WSACleanup();
        int wsaLastError = WSAGetLastError();
        throw CreateSocketException(wsaLastError);
    }
}

void ClientSocket::connectToServer(std::string host, int port)
{
    std::cout << "Connecting to server at: " << host << ":" << port << std::endl;
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    InetPton(AF_INET, std::wstring(host.begin(), host.end()).c_str(), &serverAddress.sin_addr.s_addr);

    int iResult = connect(this->socketObject, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (iResult < 0) {
   
        this->close();
        throw ConnectServerException(host, port);
    }
}

void ClientSocket::sendData(char* buffer, long bufferSize)
{
    if (send(this->socketObject, buffer, bufferSize, 0) < 0) {
        this->close();
        throw SocketSendException();
    }
}

void ClientSocket::receive(char* buffer, long bufferSize)
{
    int bytesRead = recv(this->socketObject, buffer, bufferSize, 0);
    if (bytesRead < 0) {
        this->close();
        throw SocketRecvException();
    }
}

void ClientSocket::close()
{
    closesocket(this->socketObject);
    WSACleanup();
}
