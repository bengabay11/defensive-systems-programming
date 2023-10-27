import logging
from socket import socket
from uuid import UUID
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader
from request_handlers.common import authenticate_client


class InvalidCRCRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> None:
        payload = client_socket.recv(request_header.payload_size)
        filename = payload.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        client = authenticate_client(client_id, server_db)
        logging.info(f"Received invalid crc for file - {filename} from client '{client.name}'")

