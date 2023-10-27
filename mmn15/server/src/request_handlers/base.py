import abc
from socket import socket

from dal.server_db import ServerDB
from protocol import Response, ResponseCodes
from protocol import RequestHeader

class BaseRequestHandler(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> Response:
        pass
