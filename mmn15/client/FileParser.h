#pragma once

#include <string>
#include <fstream>

#include "Structs.h"
#include "Exceptions.h"
#include "Base64Wrapper.h"

namespace fs = std::filesystem;

class FileParser
{
private:
	void stream2hex(const std::string str, size_t size, std::string& hexstr, bool capital);
public:
	ClientLoginData parseLoginFile();
	TransferFileContent parseTransferFile();
	void dumpLoginInfo(
		char clientId[Consts::CLIENT_ID_SIZE],
		char clientName[Consts::CLIENT_NAME_SIZE],
		std::string rsaPrivateKey
	);
};
