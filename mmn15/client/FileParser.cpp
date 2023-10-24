#include "FileParser.h"

TransferFileContent FileParser::parseTransferFile()
{
	std::cout << "Parsing trasfer file "<< Consts::TRANSFER_DETAILS_FILENAME << std::endl;
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

ClientLoginData FileParser::parseLoginFile()
{
	std::cout << "Parsing login file " << Consts::LOGIN_DETAILS_FILENAME << std::endl;
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

void FileParser::dumpLoginInfo(
	char clientId[Consts::CLIENT_ID_SIZE],
	char clientName[Consts::CLIENT_NAME_SIZE],
	std::string rsaPrivateKey
) {
	std::cout << "Dumping login info to " << Consts::LOGIN_DETAILS_FILENAME << std::endl;
	std::string hex;
	this->stream2hex(clientId, Consts::CLIENT_ID_SIZE, hex, true);
	std::string encodedPrivateKey = Base64Wrapper::encode(rsaPrivateKey);
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

void FileParser::stream2hex(const std::string str, size_t size, std::string& hexstr, bool capital) {
	hexstr.resize(size * 2);
	const size_t a = capital ? 'A' - 1 : 'a' - 1;

	for (size_t i = 0, c = str[0] & 0xFF; i < hexstr.size(); c = str[i / 2] & 0xFF) {
		hexstr[i++] = c > 0x9F ? (c / 16 - 9) | a : c / 16 | '0';
		hexstr[i++] = (c & 0xF) > 9 ? (c % 16 - 9) | a : c % 16 | '0';
	}
}
