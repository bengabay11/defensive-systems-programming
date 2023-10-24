from logging import debug
from socket import socket
from uuid import UUID
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader
from request_handlers.common import authenticate_client, delete_file


class InvalidCRCAbortRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        payload = client_socket.recv(request_header.payload_size)
        filename = payload.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        authenticate_client(client_id, server_db)
        delete_file(client_id, filename, server_db)
        return ResponseCodes.MESSAGE_ACCEPTED, client_id.bytes
