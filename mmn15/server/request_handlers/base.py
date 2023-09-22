import abc
from socket import socket

from dal.server_db import ServerDB
from protocol import ResponseCodes

class BaseRequestHandler(metaclass=abc.ABCMeta):
    @staticmethod
    @abc.abstractmethod
    def handle(self, client_socket: socket, payload: bytes, server_db: ServerDB) -> (ResponseCodes, bytes):
        pass
