
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "RSAWrapper.h"

namespace Consts
{
	const u_char CLIENT_VERSION = 3;

	enum RequestCodes {
		REGISTER = 1025,
		EXCHANGE_KEYS = 1026,
		RETRY_CONNECTION = 1027,
		FILE_UPLOAD = 1028,
		VALID_CRC = 1029,
		INVALID_CRC = 1030,
		INVALID_CRC_ABORT = 1031
	};

	enum ResponseCodes
	{
		REGISTRATION_SUCCESS = 2100,
		REGISTRATION_FAIL = 2101,
		PUBLIC_KEY_ACCEPTED = 2102,
		FILE_RECEIVED = 2103,
		MESSAGE_ACCEPTED = 2104,
		LOGIN_OKAY = 2105,
		LOGIN_DECLINED = 2104,
		GENERAL_SERVER_ERROR = 2107
	};

	const size_t CLIENT_ID_SIZE = 16;
	const size_t CLIENT_NAME_SIZE = 255;

	const int UPLOAD_FILE_MAX_RETRIES = 3;
	const int SEND_REQUEST_MAX_RETRIES = 3;

	const size_t FILE_NAME_SIZE = 255;

	const size_t ENCRYPTED_AES_KEY_SIZE = 128;
	const size_t DECRYPTED_AES_KEY_SIZE = 16;

	const size_t RSA_PUBLIC_KEY_SIZE = RSAPublicWrapper::KEYSIZE;

	constexpr char TRANSFER_DETAILS_FILENAME[] = "transfer.info";
	constexpr char LOGIN_DETAILS_FILENAME[] = "me.info";

	const size_t BUFFER_SIZE = 2048;
}
#endif