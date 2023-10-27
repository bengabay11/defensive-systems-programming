from socket import socket
from uuid import UUID
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import Response, ResponseCodes
from protocol import RequestHeader
from request_handlers.common import authenticate_client


class ValidCRCRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> Response:
        payload = client_socket.recv(request_header.payload_size)
        filename = payload.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        authenticate_client(client_id, server_db)
        server_db.files.update_file_verified(client_id, filename, True)
        return Response(ResponseCodes.MESSAGE_ACCEPTED, client_id.bytes)
