#include "Client.h"

Client::Client()
{
	this->clientSocket = new ClientSocket();
}

Client::~Client()
{
	delete this->clientSocket;
}

ClientHolder Client::connect(std::string host, int port, char clientName[Consts::CLIENT_NAME_SIZE]) {
	this->clientSocket->connectToServer(host, port);
	ClientHolder clientHolder{};
	std::ifstream loginFile;
	loginFile.open(Consts::LOGIN_DETAILS_FILENAME);
	if (loginFile.is_open()) {
		ClientLoginData clientLoginData = this->fileParser.parseLoginFile(Consts::LOGIN_DETAILS_FILENAME);
		LoginResponse loginResponse = this->login(clientLoginData.clientId, clientLoginData.clientName);
		this->rsaPrivateWrapper.setPrivateKey(clientLoginData.privateRSAKey);
		std::string aesKey = this->rsaPrivateWrapper.decrypt(loginResponse.encryptedAESKey, Consts::ENCRYPTED_AES_KEY_SIZE);
		memcpy(clientHolder.clientId, clientLoginData.clientId, Consts::CLIENT_ID_SIZE);
		memcpy(clientHolder.name, clientLoginData.clientName, Consts::CLIENT_NAME_SIZE);
		memcpy(clientHolder.aesKey, aesKey.c_str(), Consts::DECRYPTED_AES_KEY_SIZE);
	}
	else {
		std::cout << "Login file not exist" << std::endl;
		RegisterResponse registerResponse = this->registerClient(clientName);
		std::cout << "Registration succeded for client "<< clientName << std::endl;
		std::string aesKey = this->exchangeKeys(registerResponse.clientId, clientName);
		memcpy(clientHolder.clientId, registerResponse.clientId, Consts::CLIENT_ID_SIZE);
		memcpy(clientHolder.name, clientName, Consts::CLIENT_NAME_SIZE);
		memcpy(clientHolder.aesKey, aesKey.c_str(), Consts::DECRYPTED_AES_KEY_SIZE);
	}
	this->aesWrapper.setKey((u_char*)clientHolder.aesKey);
	return clientHolder;
}

void Client::sendRequest(
	char clientId[Consts::CLIENT_ID_SIZE],
	short requestCode, 
	char* payloadBuffer,
	long payloadSize
){
	RequesttHeader requestHeader{};
	memcpy(requestHeader.client_id, clientId, Consts::CLIENT_ID_SIZE);
	requestHeader.version = Consts::CLIENT_VERSION;
	requestHeader.code = requestCode;
	requestHeader.payload_size = payloadSize;
	char* requestHeaderBuffer = reinterpret_cast<char*>(&requestHeader);
	this->clientSocket->sendData(requestHeaderBuffer, sizeof(requestHeader));
	this->clientSocket->sendData(payloadBuffer, payloadSize);
}

ResponseHeader Client::requestResponseWorkflow(
	char clientId[Consts::CLIENT_ID_SIZE],
	short requestCode,
	char* payloadBuffer,
	long payloadSize,
	int maxRetries
)
{
	int retry;
	for (retry = 0; retry < maxRetries; retry++) {
		this->sendRequest(clientId, requestCode, payloadBuffer, payloadSize);
		ResponseHeader responseHeader = this->receiveResponseHeader();
		if (responseHeader.code == Consts::ResponseCodes::GENERAL_SERVER_ERROR) {
			std::cerr << "Server responded with an error" << std::endl;
		}
		else {
			return responseHeader;
		}
	}
	if (retry == maxRetries) {
		throw SendRequestMaxRetriesException();
	}
}

ResponseHeader Client::receiveResponseHeader()
{
	ResponseHeader responseHeader{};
	char* responseHeaderBuffer = reinterpret_cast<char*>(&responseHeader);
	this->clientSocket->receive(responseHeaderBuffer, sizeof(responseHeader));
	return responseHeader;
}

RegisterResponse Client::registerClient(char clientName[Consts::CLIENT_NAME_SIZE])
{
	std::cout << "Registering client: " << clientName << std::endl;
	RegisterRequest registerRequest {};
	memcpy(registerRequest.name, clientName, Consts::CLIENT_NAME_SIZE);
	char* registerRequestBuffer = reinterpret_cast<char*>(&registerRequest);
	char clientId[Consts::CLIENT_ID_SIZE];
	ZeroMemory(clientId, Consts::CLIENT_ID_SIZE);
	ResponseHeader responseHeader = this->requestResponseWorkflow(
		clientId,
		Consts::RequestCodes::REGISTER,
		registerRequestBuffer,
		sizeof(registerRequest)
	);
	if (responseHeader.code == Consts::ResponseCodes::REGISTRATION_SUCCESS) {
		RegisterResponse registerResponse{};
		char* registerResponseBuffer = reinterpret_cast<char*>(&registerResponse);
		this->clientSocket->receive(registerResponseBuffer, sizeof(registerResponse));
		return registerResponse;
	}
	else if (responseHeader.code == Consts::ResponseCodes::REGISTRATION_FAIL) {
		throw RegistrationFailedException(clientName);
	}
	else {
		throw UnkownResponseCodeException(responseHeader.code);
	}
}

LoginResponse Client::login(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE])
{
	std::cout << "Logging in client - " << clientName << std::endl;
	RegisterRequest registerRequest{};
	memcpy(registerRequest.name, clientName, Consts::CLIENT_NAME_SIZE);
	char* registerRequestBuffer = reinterpret_cast<char*>(&registerRequest);
	ResponseHeader responseHeader = this->requestResponseWorkflow(
		clientId,
		Consts::RequestCodes::RETRY_CONNECTION,
		registerRequestBuffer,
		sizeof(registerRequest)
	);
	if (responseHeader.code == Consts::ResponseCodes::LOGIN_OKAY) {
		std::cout << "Login accepted" << std::endl;
		LoginResponse loginResponse{};
		char* loginResponseBuffer = reinterpret_cast<char*>(&loginResponse);
		this->clientSocket->receive(loginResponseBuffer, sizeof(loginResponse));
		return loginResponse;
	}
	else if (responseHeader.code == Consts::ResponseCodes::LOGIN_DECLINED) {
		throw LoginDeclinedException();
	}
	else {
		throw UnkownResponseCodeException(responseHeader.code);
	}
}

ResponseHeader Client::sendPublicKey(
	char clientId [Consts::CLIENT_ID_SIZE],
	char clientName[Consts::CLIENT_NAME_SIZE],
	char publicKeyBuffer[RSAPublicWrapper::KEYSIZE]) {
	ExchangeKeysRequest exchangeKeysRequest{};
	memcpy(exchangeKeysRequest.name, clientName, Consts::CLIENT_NAME_SIZE);
	memcpy(exchangeKeysRequest.rsaPublicKey, publicKeyBuffer, RSAPublicWrapper::KEYSIZE);
	char* exchangeKeysRequestBuffer = reinterpret_cast<char*>(&exchangeKeysRequest);
	ResponseHeader responseHeader = this->requestResponseWorkflow(
		clientId, 
		Consts::RequestCodes::EXCHANGE_KEYS,
		exchangeKeysRequestBuffer,
		sizeof(exchangeKeysRequest)
	);
	if (responseHeader.code == Consts::ResponseCodes::PUBLIC_KEY_ACCEPTED) {
		return responseHeader;
	}
	else {
		throw UnkownResponseCodeException(responseHeader.code);
	}

}

ExchangeKeysResponse Client::receiveAESKey(char clientName[Consts::CLIENT_NAME_SIZE]) {
	ExchangeKeysResponse exchangeKeysResponse{};
	char* exchangeKeysResponseBuffer = reinterpret_cast<char*>(&exchangeKeysResponse);
	this->clientSocket->receive(exchangeKeysResponseBuffer, sizeof(exchangeKeysResponse));
	std::cout << "Server accepted public key" << std::endl;
	return exchangeKeysResponse;
}

std::string Client::exchangeKeys(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE])
{
	std::cout << "Exchanging keys with server for client " << clientName << std::endl;
	char publicKeyBuffer[RSAPublicWrapper::KEYSIZE];
	this->rsaPrivateWrapper.getPublicKey(publicKeyBuffer, RSAPublicWrapper::KEYSIZE);

	ResponseHeader responseHeader = this->sendPublicKey(clientId, clientName, publicKeyBuffer);
	ExchangeKeysResponse exchangeKeysResponse = this->receiveAESKey(clientName);
	std::string privateKey = this->rsaPrivateWrapper.getPrivateKey();
	this->fileParser.dumpLoginInfo(clientId, clientName, privateKey, Consts::LOGIN_DETAILS_FILENAME);
	this->fileParser.dumpPrivateKey(Consts::PRIVATE_KEY_FILENAME, privateKey);
	return this->rsaPrivateWrapper.decrypt(exchangeKeysResponse.aesKey, Consts::ENCRYPTED_AES_KEY_SIZE);
}

void Client::sendFileUploadRequest(
	char clientId[Consts::CLIENT_ID_SIZE],
	std::string encryptedFileContent,
	char filename[Consts::FILE_NAME_SIZE]
)
{
	FileUploadRequest fileUploadRequest{};
	fileUploadRequest.contentSize = (long)encryptedFileContent.length();
	memcpy(fileUploadRequest.fileName, filename, Consts::FILE_NAME_SIZE);
	char* fileUploadRequestBuffer = reinterpret_cast<char*>(&fileUploadRequest);
	this->sendRequest(clientId, Consts::RequestCodes::FILE_UPLOAD, fileUploadRequestBuffer, sizeof(fileUploadRequest));
	this->clientSocket->sendData(encryptedFileContent.data(), fileUploadRequest.contentSize);
}

void Client::uploadFileWithRetries(char clientId[Consts::CLIENT_ID_SIZE], std::string filePath, int maxRetries)
{
	char filename[Consts::FILE_NAME_SIZE] = {0};
	std::filesystem::path filePathObject(filePath);
	std::string filenameString = filePathObject.filename().string();
	memcpy(filename, filenameString.c_str(), filenameString.size());
	int retry;
	for (retry = 0; retry < maxRetries; retry++) {
		std::cout << "[Try #" << retry + 1 << "] ";
		bool fileUploaded = this->uploadFile(clientId, filePath, filename);
		if (fileUploaded) {
			std::cout << "Received Valid CRC. Sending Valid CRC Request" << std::endl;
			this->sendValidCRC(clientId, filename);
			break;
		}
		std::cout << "Received Invalid CRC, upload failed. Sending Invalid CRC Request" << std::endl;
		this->sendInvalidCRC(clientId, filename);

	}
	if (retry == maxRetries) {
		std::cout << "Exceeded max retries, upload failed. Sending Invalid CRC Abort Request" << std::endl;
		this->sendInvalidCRCAbort(clientId, filename);
	}
}


bool Client::uploadFile(char clientId[Consts::CLIENT_ID_SIZE], std::string filePath, char filename[Consts::FILE_NAME_SIZE])
{
	std::cout << "Uploading file to server: " << filePath << std::endl;
	std::ifstream file(filePath, std::ios::binary);
	if (file) {
		// Read file
		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		char* fileContentBuffer = new char[fileSize];
		file.read(fileContentBuffer, fileSize);

		// Calculate CRC
		this->crc.update((u_char*)fileContentBuffer, (uint32_t)fileSize);
		auto checksum = this->crc.digest();
		std::cout << "Calculated CRC for file '" << filePath << "' - " << checksum << std::endl;

		auto encryptedFileContent = this->aesWrapper.encrypt(fileContentBuffer, (unsigned int)fileSize);

		// Send upload request to server
		int retry;
		for (retry = 0; retry < Consts::SEND_REQUEST_MAX_RETRIES; retry++) {
			this->sendFileUploadRequest(clientId, encryptedFileContent, filename);
			ResponseHeader responseHeader = this->receiveResponseHeader(); 
			if (responseHeader.code == Consts::ResponseCodes::FILE_RECEIVED) {
				FileUploadResponse fileUploadResponse{};
				char* fileUploadResponseBuffer = reinterpret_cast<char*>(&fileUploadResponse);
				this->clientSocket->receive(fileUploadResponseBuffer, sizeof(fileUploadResponse));
				delete[] fileContentBuffer;
				return checksum == fileUploadResponse.checksum;
			}
			else if (responseHeader.code == Consts::ResponseCodes::GENERAL_SERVER_ERROR) {
				std::cerr << "Server responded with an error" << std::endl;
			}
			else {
				delete[] fileContentBuffer;
				throw UnkownResponseCodeException(responseHeader.code);
			}
		}
		if (retry == Consts::SEND_REQUEST_MAX_RETRIES) {
			delete[] fileContentBuffer;
			throw SendRequestMaxRetriesException();
		}
	}
	else {
		throw OpenFileException(filePath);
	}
}

void Client::sendCRCRequest(
	Consts::RequestCodes requestCode,
	char clientId[Consts::CLIENT_ID_SIZE],
	char filename[Consts::FILE_NAME_SIZE],
	bool waitForResponse
)
{
	CRCRequest crcRequest{};
	memcpy(crcRequest.filename, filename, Consts::FILE_NAME_SIZE);
	char* crcRequestBuffer = reinterpret_cast<char*>(&crcRequest);
	if (waitForResponse) {
		ResponseHeader responseHeader = this->requestResponseWorkflow(clientId, requestCode, crcRequestBuffer, sizeof(crcRequest));
		if (responseHeader.code == Consts::ResponseCodes::MESSAGE_ACCEPTED) {
			CRCResponse crcResponse{};
			char* fileUploadResponseBuffer = reinterpret_cast<char*>(&crcResponse);
			this->clientSocket->receive(fileUploadResponseBuffer, sizeof(crcResponse));
			std::cout << "Received CRC Response" << std::endl;
		}
		else {
			throw UnkownResponseCodeException(responseHeader.code);
		}
	}
	else {
		this->sendRequest(clientId, requestCode, crcRequestBuffer, sizeof(crcRequest));
	}
}

void Client::sendInvalidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE])
{
	this->sendCRCRequest(Consts::RequestCodes::INVALID_CRC, clientId, filename, false);
}

void Client::sendInvalidCRCAbort(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE])
{
	this->sendCRCRequest(Consts::RequestCodes::INVALID_CRC_ABORT, clientId, filename, true);
}

void Client::sendValidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE])
{
	this->sendCRCRequest(Consts::RequestCodes::VALID_CRC, clientId, filename, true);
}
