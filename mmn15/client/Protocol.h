#pragma once

#include <cstdio>
#include <stdint.h>
#include <Windows.h>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>

#include "Consts.h"

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
	char rsaPublicKey[Consts::RSA_PUBLIC_KEY_SIZE];
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
	char filename[Consts::FILE_NAME_SIZE];
};

#pragma pack(1)
struct RegisterResponse
{
	char clientId[Consts::CLIENT_ID_SIZE];
};

#pragma pack(1)
struct LoginResponse
{
	char clientId[Consts::CLIENT_ID_SIZE];
	char encryptedAESKey[Consts::ENCRYPTED_AES_KEY_SIZE];
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
