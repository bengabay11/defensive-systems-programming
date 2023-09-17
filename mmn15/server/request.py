
from dataclasses import dataclass
from uuid import UUID


REGISTRATION_CODE = 1025
SEND_PUBLIC_KEY_CODE = 1026
RETRY_CONNECTION_CODE = 1027
SEND_FILE_CODE = 1028
VALID_CRC_CODE = 1029
INVALID_CRC_CODE = 1030
INVALID_CRC_ABORT_CODE = 1031

@dataclass
class Request(object):
    client_id: UUID
    version: int
    code: int
    payload_size: int
    payload: any

REGISTRATION_SUCCESS_CODE = 2100
REGISTRATION_FAIL_CODE = 2101
PUBLIC_KEY_ACCEPTED_CODE = 2102
VALID_FILE_ACCEPTED_CODE = 2103
MESSAGE_ACCEPTED_CODE = 2104
RETRY_CONNECTION_ACCEPTED_CODE = 2105
RETRY_CONNECTION_DENIED_CODE = 2106
GENERAL_SERVER_ERROR_CODE = 2107


@dataclass
class Response(object):
    version: int
    code: int
    payload_size: int
    payload: any
