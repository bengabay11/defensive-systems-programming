
from dataclasses import dataclass
import pickle
from socket import socket
import struct
from uuid import UUID
from enum import Enum

import config
from exceptions import EmptyRequestHeaderException

class RequestTypes(Enum):
    REGISTER = 1025
    EXCHANGE_KEYS = 1026
    RETRY_CONNECTION = 1027
    FILE_UPLOAD = 1028
    VALID_CRC = 1029
    INVALID_CRC = 1030
    INVALID_CRC_ABORT = 1031


class ResponseCodes(Enum):
    REGISTRATION_SUCCESS = 2100
    REGISTRATION_FAIL = 2101
    PUBLIC_KEY_ACCEPTED = 2102
    VALID_FILE_ACCEPTED = 2103
    MESSAGE_ACCEPTED = 2104
    RETRY_CONNECTION_ACCEPTED = 2105
    RETRY_CONNECTION_DENIED = 2106
    GENERAL_SERVER_ERROR = 2107

REQUEST_HEADER_FORMAT = "<16sBHL"
RESPONSE_HEADER_FORMAT = "<BHL"


@dataclass
class RequestHeader(object):
    client_id: UUID
    version: int
    code: int
    payload_size: int


def get_request_header(client_socket: socket) -> RequestHeader:
    header_size = struct.calcsize(REQUEST_HEADER_FORMAT)
    header = client_socket.recv(header_size)
    if header:
        client_id, version, code, payload_size = struct.unpack(REQUEST_HEADER_FORMAT, header)
        return RequestHeader(client_id, version, code, payload_size)
    else:
        raise EmptyRequestHeaderException()


def send_response(client_socket: socket, code: ResponseCodes, payload: bytes) -> bytes:
    packed_response_header = struct.pack(RESPONSE_HEADER_FORMAT, config.VERSION, int(code), len(payload))
    client_socket.send(packed_response_header + payload)
