import abc
from socket import socket

from server.request import Request, Response

class BaseRequestHandler(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def handle(self, request: Request, client_socket: socket) -> Response:
        pass
