from socket import socket
import uuid
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import Response, ResponseCodes
from protocol import RequestHeader
import cipher
from errors import LoginFailError
from request_handlers.common import authenticate_client


class LoginRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> Response|None:
        payload = client_socket.recv(request_header.payload_size)
        client_name = payload.decode().rstrip("\x00")
        client_id = uuid.UUID(bytes=request_header.client_id)
        encrypted_aes_key = self.login_client(client_id, client_name, server_db)
        payload = client_id.bytes + encrypted_aes_key
        return Response(ResponseCodes.LOGIN_SUCCESS, payload)
    
    def login_client(self, client_id: uuid.UUID, client_name: str, server_db: ServerDB) -> bytes:
        client = authenticate_client(client_id, server_db)
        if client.name != client_name:
            raise LoginFailError(client_id, client_name)
        aes_key = cipher.generate_aes_key()
        encrypted_aes_key = cipher.encrypt_aes_key(aes_key, client.public_key)
        server_db.clients.update_client_aes_key(client_id, aes_key)
        return encrypted_aes_key
