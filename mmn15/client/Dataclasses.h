#pragma once

#include <string>

#include "Consts.h"


struct ClientHolder
{
	char clientId[Consts::CLIENT_ID_SIZE];
	char name[Consts::CLIENT_NAME_SIZE];
	char aesKey[Consts::DECRYPTED_AES_KEY_SIZE];
};

struct TransferFileContent
{
	std::string host;
	int port;
	char name[Consts::CLIENT_NAME_SIZE];
	std::string filePath;
};

struct ClientLoginData
{
	char clientName[Consts::CLIENT_NAME_SIZE];
	char clientId[Consts::CLIENT_ID_SIZE * 2];
	std::string privateRSAKey;
};
