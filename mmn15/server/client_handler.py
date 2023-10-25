from logging import info, error
from socket import socket

from dal.server_db import ServerDB
import config
import protocol
from request_router import handle_request


class ClientHandler(object):
    def __init__(self, client_socket: socket, server_db: ServerDB, socket_timeout: int = config.DEFAULT_CLIENT_SOCKET_TIMEOUT):
        self.client_socket = client_socket
        self.server_db = server_db
        if socket_timeout:
            client_socket.settimeout(socket_timeout)
        self.should_run = True

    def run(self):
        info(f"Handling new client connection {self.client_socket}")
        while self.should_run:
            try:
                request_header = protocol.get_request_header(self.client_socket)
                info(f"Received request header from {request_header}")
                response_code, payload = handle_request(self.client_socket, request_header, self.server_db)
                info(f"Sending response to {self.client_socket}, Code={response_code}")
                protocol.send_response(self.client_socket, response_code.value, payload)
            except Exception as exception:
                error(
                    f'Exception while received request from client: {exception}')
                self.close()

    def is_running(self):
        return self.should_run

    def close(self):
        info(f"Closing client connection with {self.client_socket}")
        self.should_run = False
        if self.client_socket:
            self.client_socket.close()
