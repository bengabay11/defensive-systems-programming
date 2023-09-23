from socket import socket
from uuid import UUID
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader


class ValidCRCRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        payload = client_socket.recv(request_header.payload_size)
        filename = payload.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        server_db.files.update_file_verified(client_id, filename, True)
        return ResponseCodes.MESSAGE_ACCEPTED, client_id.bytes
