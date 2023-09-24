#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "ClientSocket.h"
#include "Consts.h"
#include "Base64Wrapper.h"
#include "AESWrapper.h"
#include "ClientCRC.h"
#include "Structs.h"
#include "exceptions.h"

namespace fs = std::filesystem;

class Client
{
private:
	ClientSocket* clientSocket;
	RSAPrivateWrapper rsaPrivateWrapper;
	AESWrapper aesWrapper;

	void sendRequest(
		char clientId[Consts::CLIENT_ID_SIZE],
		short requestCode,
		char* payloadBuffer,
		long payloadSize
	);
	ResponseHeader requestResponseWorkflow(
		char clientId[Consts::CLIENT_ID_SIZE],
		short requestCode,
		char* payloadBuffer,
		long payloadSize,
		int maxRetries = Consts::SEND_REQUEST_MAX_RETRIES
	);
	ResponseHeader receiveResponseHeader();
	TransferFileContent parseTransferFile();
	ClientLoginData parseLoginFile();
	ClientHolder initialize(char newClientName[Consts::CLIENT_NAME_SIZE]);
	void dumpLoginInfo(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE]);
	ResponseHeader sendPublicKey(
		char clientId[Consts::CLIENT_ID_SIZE], 
		char clientName[Consts::CLIENT_NAME_SIZE],
		char pubkeybuff[RSAPublicWrapper::KEYSIZE]
	);
	ExchangeKeysResponse receiveAESKey(char clientName[Consts::CLIENT_NAME_SIZE]);
	void stream2hex(const std::string str, size_t size, std::string& hexstr, bool capital);
	void sendCRCRequest(Consts::RequestCodes requestCode, char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	void sendInvalidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	void sendInvalidCRCAbort(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	void sendValidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	RegisterResponse registerClient(char name[Consts::CLIENT_NAME_SIZE]);
	LoginResponse login(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE]);
	std::string exchangeKeys(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE]);
	void sendFileUploadRequest(
		char clientId[Consts::CLIENT_ID_SIZE],
		std::string encryptedFileContent,
		char filename[Consts::FILE_NAME_SIZE]
	);
	void uploadFileWithRetries(char clientId[Consts::CLIENT_ID_SIZE], std::string filePath, int maxRetries);
	bool uploadFile(char clientId[Consts::CLIENT_ID_SIZE], std::string filePath, char filename[Consts::FILE_NAME_SIZE]);
public:
	Client();
	void run();
};
