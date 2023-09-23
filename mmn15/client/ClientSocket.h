#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include "Protocol.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientSocket
{
private:
	SOCKET socketObject;
public:
	ClientSocket();
	void connectToServer(std::string host, int port);
	void sendData(char* buffer, long bufferSize);
	void receive(char* buffer, long bufferSize);
	void close();
};

