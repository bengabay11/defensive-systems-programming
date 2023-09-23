

from socket import socket
import uuid
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader
import cipher
from errors import LoginFailError


class LoginRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        payload = client_socket.recv(request_header.payload_size)
        client_name = payload.decode().rstrip("\x00")
        client_id = uuid.UUID(bytes=request_header.client_id)
        encrypted_aes_key = LoginRequestHandler.login_client(client_id, client_name, server_db)
        return ResponseCodes.LOGIN_SUCCESS, client_id.bytes + encrypted_aes_key
    
    @staticmethod
    def login_client(client_id: uuid.UUID, client_name: str, server_db: ServerDB) -> bytes:
        client = server_db.clients.get_client_by_id(client_id)
        if not client or client.name != client_name:
            raise LoginFailError(client_id, client_name)
        aes_key = cipher.generate_aes_key()
        encrypted_aes_key = cipher.encrypt_aes_key(aes_key, client.public_key)
        server_db.clients.update_client_aes_key(client_id, aes_key)
        return encrypted_aes_key
