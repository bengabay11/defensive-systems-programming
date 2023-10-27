import logging
import os
from socket import socket
from uuid import UUID
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import Response, ResponseCodes
from protocol import RequestHeader
from request_handlers.common import authenticate_client


class InvalidCRCAbortRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> Response:
        payload = client_socket.recv(request_header.payload_size)
        filename = payload.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        authenticate_client(client_id, server_db)
        self.delete_file(client_id, filename, server_db)
        return Response(ResponseCodes.MESSAGE_ACCEPTED, client_id.bytes)
    
    def delete_file(self, client_id: UUID, filename: str, server_db: ServerDB) -> None:
        file = server_db.files.get_file(client_id, filename)
        if file:
            if not file.verified:
                logging.info(f"Got invalid crc abort request for file '{file.file_name}' and its not verified. Deleting it from disk and DB.")
                server_db.files.delete_file(client_id, filename)
                if os.path.isfile(file.path_name):
                    os.remove(file.path_name)
            else:
                logging.info(f"Got invalid crc abort request for file '{file.file_name}' and its already verified before. Keeping file in the DB and disk.")


