

from socket import socket
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes


class ValidCRCRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(self, client_socket: socket, payload: bytes, server_db: ServerDB) -> (ResponseCodes, bytes):
        pass
