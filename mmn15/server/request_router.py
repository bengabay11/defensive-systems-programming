from logging import error
from socket import socket

from errors import BaseServerError, UnsupportedRequestCodeException
from protocol import RequestTypes, RequestHeader
from request_handlers.exchange_keys import ExchangeKeysRequestHandler
from request_handlers.file_upload import FileUploadRequestHandler
from request_handlers.invalid_crc import InvalidCRCRequestHandler
from request_handlers.invalid_crc_abort import InvalidCRCAbortRequestHandler
from request_handlers.register import RegisterRequestHandler
from request_handlers.valid_crc import ValidCRCRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from request_handlers.login import LoginRequestHandler

REQUEST_HANDLERS = {
    RequestTypes.REGISTER: RegisterRequestHandler,
    RequestTypes.EXCHANGE_KEYS: ExchangeKeysRequestHandler,
    RequestTypes.LOGIN: LoginRequestHandler,
    RequestTypes.FILE_UPLOAD: FileUploadRequestHandler,
    RequestTypes.VALID_CRC: ValidCRCRequestHandler,
    RequestTypes.INVALID_CRC: InvalidCRCRequestHandler,
    RequestTypes.INVALID_CRC_ABORT: InvalidCRCAbortRequestHandler
}


def handle_request(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, any):
    request_type = RequestTypes(request_header.code)
    request_handler = REQUEST_HANDLERS.get(request_type)
    if not request_handler:
        raise UnsupportedRequestCodeException(request_header.code)
    try:
        return request_handler.handle(client_socket, request_header, server_db)
    except BaseServerError as exception:
        error(f"Error while handling request. request code: {request_header.code}, error: {exception}");
        return exception.error_code(), b''
    except Exception as exception:
        error(f"Unknown Error while handling request. request code: {request_header.code}, error: {exception}");
        return ResponseCodes.GENERAL_SERVER_ERROR, b''
