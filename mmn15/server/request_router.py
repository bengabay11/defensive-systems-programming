from logging import error

from exceptions import UnsupportedRequestCodeException
from protocol import RequestTypes, RequestHeader
from request_handlers.exchange_keys import ExchangeKeysRequestHandler
from request_handlers.file_upload import FileUploadRequestHandler
from request_handlers.invalid_crc import InvalidCRCRequestHandler
from request_handlers.invalid_crc_abort import InvalidCRCAbortRequestHandler
from request_handlers.register import RegisterRequestHandler
from request_handlers.retry_connection import RetryConnectionRequestHandler
from request_handlers.valid_crc import ValidCRCRequestHandler
from dal.server_db import ServerDB
from server_errors import BaseServerError
from protocol import ResponseCodes

REQUEST_HANDLERS = {
    RequestTypes.REGISTER: RegisterRequestHandler,
    RequestTypes.EXCHANGE_KEYS: ExchangeKeysRequestHandler,
    RequestTypes.RETRY_CONNECTION: RetryConnectionRequestHandler,
    RequestTypes.FILE_UPLOAD: FileUploadRequestHandler,
    RequestTypes.VALID_CRC: ValidCRCRequestHandler,
    RequestTypes.INVALID_CRC: InvalidCRCRequestHandler,
    RequestTypes.INVALID_CRC_ABORT: InvalidCRCAbortRequestHandler
}


def handle_request(request_header: RequestHeader, payload: bytes, server_db: ServerDB) -> (ResponseCodes, any):
    request_type = RequestTypes(request_header.code)
    request_handler = REQUEST_HANDLERS.get(request_type)
    if not request_handler:
        raise UnsupportedRequestCodeException(request_header.code)
    try:
        return request_handler.handle(None, payload, server_db)
    except BaseServerError as exception:
        error(f"Error while handling request. request code: {request_header.code}, error: {exception}");
        return exception.error_code(), b''
    except Exception as exception:
        error(f"Unknown Error while handling request. request code: {request_header.code}, error: {exception}");
        return ResponseCodes.GENERAL_SERVER_ERROR, b''
