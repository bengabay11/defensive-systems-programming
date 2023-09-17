

from socket import socket
from server.request import Request, Response
from server.request_handlers.base import BaseRequestHandler


class LoginRequestHandler(BaseRequestHandler):
    def handle(self, request: Request, client_socket: socket) -> Response:
        pass

