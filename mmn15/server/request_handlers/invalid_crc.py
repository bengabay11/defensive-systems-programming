

from logging import debug
import os
from socket import socket
from uuid import UUID
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader
from request_handlers.invalid_crc_abort import InvalidCRCAbortRequestHandler


class InvalidCRCRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        payload = client_socket.recv(request_header.payload_size)
        filename = payload.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        InvalidCRCRequestHandler.delete_file(client_id, filename, server_db)
        return ResponseCodes.MESSAGE_ACCEPTED, client_id.bytes

    @staticmethod
    def delete_file(client_id: UUID, filename: str, server_db: ServerDB) -> None:
        file = server_db.files.get_file(client_id, filename)
        if file:
            base_folder = os.path.dirname(file.path_name)
            debug(f"Invalid CRC for file - '{filename}'. Removing base folder - '{base_folder}' and deleting the file from DB")
            server_db.files.delete_file(client_id, filename)
            if os.path.isfile(file.path_name):
                os.remove(file.path_name)
            if os.path.isdir(base_folder):
                os.rmdir(base_folder)