

from socket import socket
import uuid
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
import cipher
from protocol import RequestHeader
from errors import ClientNotFoundError, UnauthorizedClientError


RSA_PUBLIC_KEY_LENGTH = 160
CLIENT_NAME_LENGTH = 255


class ExchangeKeysRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        payload = client_socket.recv(request_header.payload_size)
        client_name = payload[:CLIENT_NAME_LENGTH].decode().rstrip("\x00")
        client_id = uuid.UUID(bytes=request_header.client_id)
        public_key = payload[CLIENT_NAME_LENGTH:CLIENT_NAME_LENGTH + RSA_PUBLIC_KEY_LENGTH]
        encrypted_aes_key = ExchangeKeysRequestHandler.exchange_keys(client_id, client_name, public_key, server_db)
        return ResponseCodes.PUBLIC_KEY_ACCEPTED, client_id.bytes + encrypted_aes_key
    
    @staticmethod
    def exchange_keys(client_id: uuid.UUID, client_name: str, public_key: str, server_db: ServerDB):
        client = server_db.clients.get_client_by_id(client_id)
        if client:
            if client.name != client_name:
                raise UnauthorizedClientError(client_name, client.name)
            aes_key = cipher.generate_aes_key()
            encrypted_aes_key = cipher.encrypt_aes_key(aes_key, public_key)
            server_db.clients.update_client_public_key(client_name, public_key)
            server_db.clients.update_client_aes_key(client_id, aes_key)
            return encrypted_aes_key
        else:
            raise ClientNotFoundError(client_id)
