import abc
from socket import socket

from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader

class BaseRequestHandler(metaclass=abc.ABCMeta):
    @staticmethod
    @abc.abstractmethod
    def handle(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        pass
