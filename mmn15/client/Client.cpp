#include "Client.h"

Client::Client()
{
	this->clientSocket = new ClientSocket();
}

void Client::run()
{
	std::cout << "Parsing trasfer file" << std::endl;
	TransferFileContent transferFileContent = this->parseTransferFile();
	std::cout << "Connecting to server at: " << transferFileContent.host << ":" << transferFileContent.port << std::endl;
	this->clientSocket->connectToServer(transferFileContent.host, transferFileContent.port);
	ClientHolder clientHolder = this->initialize(transferFileContent.name);
	this->aesWrapper.setKey((u_char*)clientHolder.aesKey);
	this->uploadFileWithRetries(clientHolder.clientId, transferFileContent.filePath, Consts::UPLOAD_FILE_MAX_RETRIES);
}

ClientHolder Client::initialize(char fallbackClientName[Consts::CLIENT_NAME_SIZE]) {
	ClientHolder clientHolder{};
	std::ifstream loginFile;
	loginFile.open(Consts::LOGIN_DETAILS_FILENAME);
	if (loginFile.is_open()) {
		std::cout << "Parsing login file" << std::endl;
		ClientLoginData clientLoginData = this->parseLoginFile();
		std::cout << "Logging in client - " << clientLoginData.clientName << std::endl;
		LoginResponse loginResponse = this->login(clientLoginData.clientId, clientLoginData.clientName);
		this->rsaPrivateWrapper.setPrivateKey(clientLoginData.privateRSAKey);
		std::string aesKey = this->rsaPrivateWrapper.decrypt(loginResponse.encryptedAESKey, Consts::ENCRYPTED_AES_KEY_SIZE);
		memcpy(clientHolder.clientId, clientLoginData.clientId, Consts::CLIENT_ID_SIZE);
		memcpy(clientHolder.name, clientLoginData.clientName, Consts::CLIENT_NAME_SIZE);
		memcpy(clientHolder.aesKey, aesKey.c_str(), Consts::DECRYPTED_AES_KEY_SIZE);
	}
	else {
		std::cout << "Login file not exist. Registering client: " << fallbackClientName << std::endl;
		RegisterResponse registerResponse = this->registerClient(fallbackClientName);
		std::cout << "Registration succeded" << std::endl;
		std::cout << "Exchanging keys with server" << std::endl;
		std::string aesKey = this->exchangeKeys(registerResponse.clientId, fallbackClientName);
		memcpy(clientHolder.clientId, registerResponse.clientId, Consts::CLIENT_ID_SIZE);
		memcpy(clientHolder.name, fallbackClientName, Consts::CLIENT_NAME_SIZE);
		memcpy(clientHolder.aesKey, aesKey.c_str(), Consts::DECRYPTED_AES_KEY_SIZE);
		return clientHolder;
	}
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

TransferFileContent Client::parseTransferFile()
{
	std::ifstream transferFile;
	transferFile.open(Consts::TRANSFER_DETAILS_FILENAME);
	if (transferFile.is_open()) {
		TransferFileContent transferFileContent;
		std::string address, clientName;
		std::getline(transferFile, address);
		std::getline(transferFile, clientName);
		std::getline(transferFile, transferFileContent.filePath);
		ZeroMemory(transferFileContent.name, sizeof(transferFileContent.name));
		strcpy_s(transferFileContent.name, clientName.size() + 1, clientName.c_str());
		std::vector<std::string> tokens;
		boost::split(tokens, address, boost::is_any_of(":"));
		transferFileContent.host = tokens[0];
		transferFileContent.port = std::stoi(tokens[1]);
		return transferFileContent;
	}
	else {
		throw OpenFileException(Consts::TRANSFER_DETAILS_FILENAME);
	}
}

ClientLoginData Client::parseLoginFile()
{
	std::fstream meFile;
	meFile.open(Consts::LOGIN_DETAILS_FILENAME);
	if (meFile.is_open()) {
		ClientLoginData clientLoginData{};
		std::string clientName, encodedClientId, encodedPrivateRSAKey;
		std::getline(meFile, clientName);
		std::getline(meFile, encodedClientId);
		std::getline(meFile, encodedPrivateRSAKey);
		strcpy_s(clientLoginData.clientName, clientName.size() + 1, clientName.c_str());
		memcpy(clientLoginData.clientId, boost::algorithm::unhex(encodedClientId).c_str(), Consts::CLIENT_ID_SIZE);
		std::string privateRSAKey = Base64Wrapper::decode(encodedPrivateRSAKey);
		clientLoginData.privateRSAKey.assign(privateRSAKey);
		return clientLoginData;
	}
	else {
		throw OpenFileException(Consts::LOGIN_DETAILS_FILENAME);
	}
}

void Client::dumpLoginInfo(char clientId[Consts::CLIENT_ID_SIZE], char clientName[Consts::CLIENT_NAME_SIZE]) {
	std::cout << "Dumping login info to " << Consts::LOGIN_DETAILS_FILENAME << std::endl;
	std::string hex;
	this->stream2hex(clientId, Consts::CLIENT_ID_SIZE, hex, true);
	std::string encodedPrivateKey = Base64Wrapper::encode(this->rsaPrivateWrapper.getPrivateKey());
	size_t pos;
	while ((pos = encodedPrivateKey.find("\n")) != std::string::npos) {
		encodedPrivateKey.replace(pos, 1, "");
	}

	std::fstream loginFile(Consts::LOGIN_DETAILS_FILENAME, std::ios::out);
	if (loginFile.is_open()) {
		loginFile << clientName << std::endl;
		loginFile << std::string(hex) << std::endl;
		loginFile << encodedPrivateKey;
		loginFile.close();
	}
	else {
		throw OpenFileException(Consts::LOGIN_DETAILS_FILENAME);
	}
}

RegisterResponse Client::registerClient(char clientName[Consts::CLIENT_NAME_SIZE])
{
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
	char publicKeyBuffer[RSAPublicWrapper::KEYSIZE];
	this->rsaPrivateWrapper.getPublicKey(publicKeyBuffer, RSAPublicWrapper::KEYSIZE);
	std::string privateKey = this->rsaPrivateWrapper.getPrivateKey();

	ResponseHeader responseHeader = this->sendPublicKey(clientId, clientName, publicKeyBuffer);
	ExchangeKeysResponse exchangeKeysResponse = this->receiveAESKey(clientName);
	this->dumpLoginInfo(clientId, clientName);
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
	ZeroMemory(filename, sizeof(filename));
	std::filesystem::path filePathObject(filePath);
	std::string x = filePathObject.filename().string();
	memcpy(filename, filePathObject.filename().string().c_str(), x.size());
	int retry;
	for (retry = 0; retry < maxRetries; retry++) {
		std::cout << "[Try #" << retry + 1 << "] Uploading file to server: " << filePath << std::endl;
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
	std::ifstream file(filePath, std::ios::binary);
	if (file) {
		// Read file
		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		char* buffer = new char[fileSize];
		file.read(buffer, fileSize);

		// Calculate CRC
		CRC crc;
		crc.update((u_char*)buffer, (uint32_t)fileSize);
		auto checksum = crc.digest();

		auto encryptedFileContent = this->aesWrapper.encrypt(buffer, (unsigned int)fileSize);

		// Send upload request to server
		int retry;
		for (retry = 0; retry < Consts::SEND_REQUEST_MAX_RETRIES; retry++) {
			this->sendFileUploadRequest(clientId, encryptedFileContent, filename);
			ResponseHeader responseHeader = this->receiveResponseHeader(); 
			if (responseHeader.code == Consts::ResponseCodes::FILE_RECEIVED) {
				FileUploadResponse fileUploadResponse{};
				char* fileUploadResponseBuffer = reinterpret_cast<char*>(&fileUploadResponse);
				this->clientSocket->receive(fileUploadResponseBuffer, sizeof(fileUploadResponse));
				return checksum == fileUploadResponse.checksum;
			}
			else if (responseHeader.code == Consts::ResponseCodes::GENERAL_SERVER_ERROR) {
				std::cerr << "Server responded with an error" << std::endl;
			}
			else {
				throw UnkownResponseCodeException(responseHeader.code);
			}
		}
		if (retry == Consts::SEND_REQUEST_MAX_RETRIES) {
			throw SendRequestMaxRetriesException();
		}
	}
	else {
		throw OpenFileException(filePath);
	}
}

void Client::stream2hex(const std::string str, size_t size, std::string& hexstr, bool capital) {
	hexstr.resize(size * 2);
	const size_t a = capital ? 'A' - 1 : 'a' - 1;

	for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF) {
		hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
		hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
	}
}

void Client::sendCRCRequest(
	Consts::RequestCodes requestCode,
	char clientId[Consts::CLIENT_ID_SIZE],
	char filename[Consts::FILE_NAME_SIZE]
)
{
	CRCRequest crcRequest{};
	memcpy(crcRequest.filename, filename, Consts::FILE_NAME_SIZE);
	char* crcRequestBuffer = reinterpret_cast<char*>(&crcRequest);
	ResponseHeader responseHeader = this->requestResponseWorkflow(clientId, requestCode, crcRequestBuffer, sizeof(crcRequest));
	if (responseHeader.code == Consts::ResponseCodes::MESSAGE_ACCEPTED) {
		CRCResponse crcResponse {};
		char* fileUploadResponseBuffer = reinterpret_cast<char*>(&crcResponse);
		this->clientSocket->receive(fileUploadResponseBuffer, sizeof(crcResponse));
		std::cout << "Received CRC Response" << std::endl;
	}
	else {
		throw UnkownResponseCodeException(responseHeader.code);
	}
}

void Client::sendInvalidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE])
{
	this->sendCRCRequest(Consts::RequestCodes::INVALID_CRC, clientId, filename);
}

void Client::sendInvalidCRCAbort(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE])
{
	this->sendCRCRequest(Consts::RequestCodes::INVALID_CRC_ABORT, clientId, filename);
}

void Client::sendValidCRC(char clientId[Consts::CLIENT_ID_SIZE], char filename[Consts::FILE_NAME_SIZE])
{
	this->sendCRCRequest(Consts::RequestCodes::VALID_CRC, clientId, filename);
}

