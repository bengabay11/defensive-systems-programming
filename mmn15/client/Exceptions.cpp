#include "Exceptions.h"

SendRequestMaxRetriesException::SendRequestMaxRetriesException(): 
	runtime_error("Send request exceeded max retries") {}

UnkownResponseCodeException::UnkownResponseCodeException(short responseCode):
	runtime_error("Unkown response code - " + responseCode), responseCode(responseCode) {}

LoginDeclinedException::LoginDeclinedException(): runtime_error("Server declined the login") {}

RegistrationFailedException::RegistrationFailedException(std::string clientName):
	runtime_error("Registration faield, Client with the same name already exists - " + clientName), clientName(clientName) {}

OpenFileException::OpenFileException(std::string filePath) : runtime_error("Couldn't open file - " + filePath), filePath(filePath) {}

std::string ConnectServerException::generateErrorMessage(const std::string& host, int port)
{
	std::ostringstream message;
	message << "Failed to connect to server at " << host << ":" << port;
	return message.str();
}

ConnectServerException::ConnectServerException(std::string host, int port)
	: std::runtime_error(generateErrorMessage(host, port)), host(host), port(port) {}

SocketSendException::SocketSendException(): runtime_error("Failed to send data to the server") {}

SocketRecvException::SocketRecvException(): runtime_error("Failed to receive data from the server") {}

InitWinsockException::InitWinsockException(): runtime_error("Failed to initialize winsock") {}

CreateSocketException::CreateSocketException(int WSALastError): 
	runtime_error("Failed to create socket. WSA last error -" + WSALastError), WSALastError(WSALastError) {}
