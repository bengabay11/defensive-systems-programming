

from dataclasses import dataclass
import datetime
from socket import socket
import uuid

from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from dal.models.client import Client
from server_errors import RegistrationFailError


class RegisterRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(client_socket: socket, payload: bytes, server_db: ServerDB) -> (ResponseCodes, bytes):
        client_name = payload.decode().rstrip("\x00")
        client_id = RegisterRequestHandler.register_client(client_name, server_db)
        return ResponseCodes.REGISTRATION_SUCCESS, client_id.bytes
    
    @staticmethod
    def register_client(client_name: str, server_db: ServerDB):
        if server_db.clients.client_name_exists(client_name):
            raise RegistrationFailError(client_name)
        else:
            client_id = uuid.uuid4()
            new_client = Client(id=str(client_id), name=client_name, public_key=b'', last_seen=datetime.datetime.now(), aes_key=b'')
            server_db.clients.add_client(new_client)
            return client_id                 
