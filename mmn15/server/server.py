from logging import error, info
from socket import AF_INET, SOCK_STREAM, SOL_SOCKET, socket, SO_REUSEADDR
from threading import Thread

import protocol
from request_router import handle_request
from dal.server_db import ServerDB


class Server(object):
    def __init__(self, host: str, port: int, server_db: ServerDB):
        self.host = host
        self.port = port
        self.server_db = server_db
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        self.socket.bind((self.host, self.port))

    def run(self, max_clients):
        self.socket.listen(max_clients)
        while True:
            client_socket, address = self.socket.accept()
            info(f'Accepted new connection: {address}')
            Thread(target=self.handle_client, args=(client_socket, self.server_db)).start()

    def handle_client(self, client_socket: socket, server_db: ServerDB, socket_timeout=60):
        client_socket.settimeout(socket_timeout)
        receive_requests = True
        while receive_requests:
            try:
                request_header = protocol.get_request_header(client_socket)
                info(f"Received request header from {request_header}")
                payload = client_socket.recv(request_header.payload_size)
                response_code, payload = handle_request(request_header, payload, server_db)
                info(f"Sending response to {client_socket}, Code={response_code}")
                protocol.send_response(client_socket, response_code.value, payload)
            except Exception as exception:
                error(
                    f'Closing client connection due to an error: {exception}')
                client_socket.close()
                receive_requests = False
