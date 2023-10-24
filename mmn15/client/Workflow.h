#pragma once

#include "Client.h"
#include "FileParser.h"

class Workflow
{
private:
	Client* client;
	FileParser fileParser;
public:
	Workflow();
	~Workflow();
	void run();
};

