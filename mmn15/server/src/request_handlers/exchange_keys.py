from socket import socket
import struct
import uuid
from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import Response, ResponseCodes
import cipher
from protocol import RequestHeader
from errors import UnauthorizedClientError
from request_handlers.common import authenticate_client


METADATA_FORMAT = "<255s160s"


class ExchangeKeysRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> Response|None:
        payload = client_socket.recv(request_header.payload_size)
        client_name_bytes, public_key = struct.unpack(METADATA_FORMAT, payload)
        client_name = client_name_bytes.decode().rstrip("\x00")
        client_id = uuid.UUID(bytes=request_header.client_id)
        encrypted_aes_key = self.exchange_keys(client_id, client_name, public_key, server_db)
        payload = client_id.bytes + encrypted_aes_key
        return Response(ResponseCodes.PUBLIC_KEY_ACCEPTED, payload)
    
    def exchange_keys(self, client_id: uuid.UUID, client_name: str, public_key: str, server_db: ServerDB) -> bytes:
        client = authenticate_client(client_id, server_db)
        if client.name != client_name:
            raise UnauthorizedClientError(client_name, client.name)
        aes_key = cipher.generate_aes_key()
        encrypted_aes_key = cipher.encrypt_aes_key(aes_key, public_key)
        server_db.clients.update_client_public_key(client_name, public_key)
        server_db.clients.update_client_aes_key(client_id, aes_key)
        return encrypted_aes_key
