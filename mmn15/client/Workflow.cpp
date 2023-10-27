#include "Workflow.h"

Workflow::Workflow()
{
	this->client = new Client();
}

Workflow::~Workflow()
{
	delete this->client;
}

void Workflow::run()
{
	TransferFileContent transferFileContent = this->fileParser.parseTransferFile(Consts::TRANSFER_DETAILS_FILENAME);
	ClientHolder clientHolder = this->client->connect(transferFileContent.host, transferFileContent.port, transferFileContent.name);
	this->client->uploadFileWithRetries(clientHolder.clientId, transferFileContent.filePath, Consts::UPLOAD_FILE_MAX_RETRIES);
}
