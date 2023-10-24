from socket import socket
from uuid import UUID
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader
from request_handlers.common import authenticate_client, delete_file


class InvalidCRCRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        client_id = UUID(bytes=request_header.client_id)
        authenticate_client(client_id, server_db)
        return ResponseCodes.MESSAGE_ACCEPTED, client_id.bytes
