#pragma once

#include <filesystem>

#include "ClientSocket.h"
#include "Consts.h"
#include "AESWrapper.h"
#include "CRC.h"
#include "Structs.h"
#include "exceptions.h"
#include "FileParser.h"

namespace fs = std::filesystem;

class Client
{
private:
	ClientSocket* clientSocket;
	RSAPrivateWrapper rsaPrivateWrapper;
	AESWrapper aesWrapper;
	FileParser fileParser;
	CRC crc;

	RegisterResponse registerClient(char name[Consts::CLIENT_NAME_SIZE]);
	LoginResponse login(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE]);
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
	ResponseHeader sendPublicKey(
		char clientId[Consts::CLIENT_ID_SIZE], 
		char clientName[Consts::CLIENT_NAME_SIZE],
		char pubkeybuff[RSAPublicWrapper::KEYSIZE]
	);
	ExchangeKeysResponse receiveAESKey(char clientName[Consts::CLIENT_NAME_SIZE]);
	std::string exchangeKeys(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE]);
	void sendCRCRequest(Consts::RequestCodes requestCode, char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	void sendInvalidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	void sendInvalidCRCAbort(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	void sendValidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE]);
	void sendFileUploadRequest(
		char clientId[Consts::CLIENT_ID_SIZE],
		std::string encryptedFileContent,
		char filename[Consts::FILE_NAME_SIZE]
	);
public:
	Client();
	~Client();
	/**
	* @brief Connect the client to the server and initialize secure communication protocol.
	*
	* The function will try to parse a login file if the client has logged in before.
	* 
	* If the client with the given name is never logged in before, the function will try to register it.
	* 
	* @param host - The host of the server
	* @param port - The port of the server
	* @param clientName - The name of client to connect
	*/
	ClientHolder connect(std::string host, int port, char clientName[Consts::CLIENT_NAME_SIZE]);
	/**
	* @brief Upload local file to the server with retries mechanism
	*
	* @param clientId - The ID of the client
	* @param filePath - The full path of  the local file
	* @param maxRetries - The amount of retries the client should try and upload the file, before it throws and exception
	*/
	void uploadFileWithRetries(char clientId[Consts::CLIENT_ID_SIZE], std::string filePath, int maxRetries);
	/**
	* @brief Upload local file to the server
	*
	* @param clientId - The id of the client
	* @param filePath - The full path of  the local file
	* @param filename - The name of the file
	*/
	bool uploadFile(char clientId[Consts::CLIENT_ID_SIZE], std::string filePath, char filename[Consts::FILE_NAME_SIZE]);
};
