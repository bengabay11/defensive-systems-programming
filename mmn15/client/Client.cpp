#include "Client.h"
#include "Base64Wrapper.h"

Client::Client()
{
	this->clientSocket = new ClientSocket();
}

void Client::run()
{
	std::cout << "Parsing trasfer file" << std::endl;
	TransferFileContent transferFileContent = this->parseTransferFile();
	this->clientSocket->connectToServer(transferFileContent.host, transferFileContent.port);
	this->registerClient(transferFileContent.name);
	this->exchangeKeys(transferFileContent.name);
	this->uploadFile(transferFileContent.filePath);
	this->abort();
}

void Client::sendRequest(char* payloadBuffer, long payloadSize)
{
	// Send request header
	RequesttHeader requestHeader{};
	memcpy(requestHeader.client_id, this->clientId, Consts::CLIENT_ID_SIZE);
	requestHeader.version = Consts::CLIENT_VERSION;
	requestHeader.code = Consts::RequestCodes::REGISTER;
	requestHeader.payload_size = payloadSize;
	char* requestHeaderBuffer = reinterpret_cast<char*>(&requestHeader);
	this->clientSocket->sendData(requestHeaderBuffer, sizeof(requestHeader));

	// Send payload
	this->clientSocket->sendData(payloadBuffer, payloadSize);
}

ResponseHeader Client::receiveResponseHeader()
{
	// Receive response header
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
		ZeroMemory(transferFileContent.name, Consts::CLIENT_NAME_SIZE);
		std::string address, clientName;
		std::getline(transferFile, address);
		std::getline(transferFile, clientName);
		std::getline(transferFile, transferFileContent.filePath);
		strcpy_s(transferFileContent.name, clientName.size() + 1, clientName.c_str());

		std::vector<std::string> tokens;
		boost::split(tokens, address, boost::is_any_of(":"));
		transferFileContent.host = tokens[0];
		transferFileContent.port = std::stoi(tokens[1]);
		return transferFileContent;
	}
	else {
		std::cerr << "Couldn't open transfer file" << std::endl;
	}
}

ClientLoginData Client::parseLoginFile()
{
	std::ifstream meFile;
	meFile.open(Consts::CLIENT_DETAILS_FILENAME);
	if (meFile.is_open()) {
		ClientLoginData clientLoginData{};
		std::string clientName, clientId, encodedAESKey;
		std::getline(meFile, clientName);
		std::getline(meFile, clientId);
		std::getline(meFile, encodedAESKey);
		std::string aesKey = Base64Wrapper::decode(aesKey);
		strcpy_s(clientLoginData.name, clientName.size() + 1, clientName.c_str());
		strcpy_s(clientLoginData.id , clientId.size() + 1, clientId.c_str());
		strcpy_s(clientLoginData.aesKey, aesKey.size() + 1, aesKey.c_str());
		std::cout << "Successfully parsed client login file. ID: " << clientLoginData.id << ". Name: " << clientLoginData.name << std::endl;
		return clientLoginData;
	}
	else {
		std::cerr << "Couldn't open open login file" << std::endl;
	}
}

void Client::registerClient(char name[Consts::CLIENT_NAME_SIZE])
{
	std::cout << "Registering client: " << name << std::endl;
	RegisterRequest registerRequest {};
	memcpy(registerRequest.name, name, Consts::CLIENT_NAME_SIZE);
	char* registerRequestBuffer = reinterpret_cast<char*>(&registerRequest);
	this->sendRequest(registerRequestBuffer, sizeof(registerRequest));
	ResponseHeader responseHeader = this->receiveResponseHeader();
	if (responseHeader.code == Consts::ResponseCodes::REGISTRATION_SUCCESS) {
		RegisterResponse registerResponse{};
		char* registerResponseBuffer = reinterpret_cast<char*>(&registerResponse);
		this->clientSocket->receive(registerResponseBuffer, responseHeader.payloadSize);
		std::cout << "Registration succeded. Client ID: " << registerResponse.client_id << std::endl;
		memcpy(this->clientId, registerResponse.client_id, Consts::CLIENT_ID_SIZE);
	}
	else if (responseHeader.code == Consts::ResponseCodes::REGISTRATION_FAIL) {
		std::cerr << "Registration faield, Client with the same name already exists." << std::endl;
	}

}

void Client::exchangeKeys(char clientName[Consts::CLIENT_NAME_SIZE])
{
	RSAPrivateWrapper rsapriv;
	char pubkeybuff[RSAPublicWrapper::KEYSIZE];
	rsapriv.getPublicKey(pubkeybuff, RSAPublicWrapper::KEYSIZE);
	std::string encodedPrivateKey = Base64Wrapper::encode(rsapriv.getPrivateKey());

	ExchangeKeysRequest exchangeKeysRequest{};
	memcpy(exchangeKeysRequest.name, clientName, Consts::CLIENT_NAME_SIZE);
	memcpy(exchangeKeysRequest.rsa_public_key, pubkeybuff, RSAPublicWrapper::KEYSIZE);
	char* exchangeKeysRequestBuffer = reinterpret_cast<char*>(&exchangeKeysRequest);
	this->sendRequest(exchangeKeysRequestBuffer, sizeof(exchangeKeysRequest));

	ResponseHeader responseHeader = this->receiveResponseHeader();
	if (responseHeader.code == Consts::ResponseCodes::PUBLIC_KEY_ACCEPTED) {
		ExchangeKeysResponse exchangeKeysResponse{};
		char* exchangeKeysResponseBuffer = reinterpret_cast<char*>(&exchangeKeysResponse);
		this->clientSocket->receive(exchangeKeysResponseBuffer, responseHeader.payloadSize);
		std::cout << "Server accepted public key" << std::endl;
		dumpLoginInfo();
	}
	else {
		std::cerr << "Server failed to accept public key" << std::endl;
	}
}

void Client::dumpLoginInfo() {

}

void Client::uploadFile(std::string filePath)
{
}

void Client::abort()
{
	// Send abort to server
	this->clientSocket->close();
}
