#include "Exceptions.h"

GeneralServerErrorException::GeneralServerErrorException() : runtime_error("Got general server error") {}

UnkownResponseCodeException::UnkownResponseCodeException(short responseCode):
	runtime_error("Unkown response code - " + responseCode), responseCode(responseCode) {}

LoginDeclinedException::LoginDeclinedException(): runtime_error("Server declined the login") {}

RegistrationFailedException::RegistrationFailedException(std::string clientName):
	runtime_error("Registration faield, Client with the same name already exists - " + clientName), clientName(clientName) {}

OpenFileFailException::OpenFileFailException(std::string filePath): runtime_error("Couldn't open file - " + filePath), filePath(filePath) {}
