#pragma once

#include "Consts.h"
#include <cstdio>
#include <stdint.h>
#include <Windows.h>
#include "Structs.h"

struct ClientLoginData
{
	char id[Consts::CLIENT_ID_SIZE];
	char name[Consts::CLIENT_NAME_SIZE];
	char aesKey[Consts::DECRYPTED_AES_KEY_SIZE];
};

struct TransferFileContent
{
	std::string host;
	int port;
	char name[Consts::CLIENT_NAME_SIZE];
	std::string filePath;
};

#pragma pack(1)
struct RequesttHeader
{
	char client_id[Consts::CLIENT_ID_SIZE];
	char version;
	short code;
	long payload_size;
};

#pragma pack(1)
struct ResponseHeader
{
	uint8_t version;
	uint16_t code;
	uint32_t payloadSize;
};

#pragma pack(1)
struct RegisterRequest
{
	char name[Consts::CLIENT_NAME_SIZE];
};

#pragma pack(1)
struct ExchangeKeysRequest
{
	char name[Consts::CLIENT_NAME_SIZE];
	char rsa_public_key[Consts::RSA_PUBLIC_KEY_SIZE];
};

#pragma pack(1)
struct FileUploadRequest
{
	long contentSize;
	char fileName[Consts::FILE_NAME_SIZE];
};

#pragma pack(1)
struct CRCRequest
{
	char fileName[Consts::FILE_NAME_SIZE];
};

#pragma pack(1)
struct RegisterResponse
{
	char client_id[Consts::CLIENT_ID_SIZE];
};

#pragma pack(1)
struct LoginResponse
{
	char client_id[Consts::CLIENT_ID_SIZE];
	char aes_key[Consts::ENCRYPTED_AES_KEY_SIZE];
};

#pragma pack(1)
struct ExchangeKeysResponse
{
	char clientId[Consts::CLIENT_ID_SIZE];
	char aesKey[Consts::ENCRYPTED_AES_KEY_SIZE];
};

#pragma pack(1)
struct FileUploadResponse
{
	char client_id[Consts::CLIENT_ID_SIZE];
	u_long contentSize;
	char fileName[Consts::FILE_NAME_SIZE];
	u_long checksum;
};
