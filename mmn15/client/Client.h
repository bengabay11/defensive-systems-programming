#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "ClientSocket.h"
#include "Consts.h"
#include "Base64Wrapper.h"

class Client
{
private:
	ClientSocket* clientSocket;
	char clientId[Consts::CLIENT_ID_SIZE];
	void sendRequest(char* payloadBuffer, long payloadSize);
	ResponseHeader receiveResponseHeader();
	TransferFileContent parseTransferFile();
	ClientLoginData parseLoginFile();
	void dumpLoginInfo();
public:
	Client();
	void run();
	void registerClient(char name[Consts::CLIENT_NAME_SIZE]);
	void exchangeKeys(char clientName[Consts::CLIENT_NAME_SIZE]);
	void uploadFile(std::string filePath);
	void abort();
};

