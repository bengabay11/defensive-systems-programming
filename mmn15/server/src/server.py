import logging
import os
from socket import AF_INET, SOCK_STREAM, SOL_SOCKET, socket, SO_REUSEADDR
from threading import Thread
import threading

from dal.server_db import ServerDB
import config
from client_handler import ClientHandler


class Server(object):
    def __init__(self, host: str, port: int, server_db: ServerDB, server_socket_timeout: int = config.DEFAULT_SERVER_SOCKET_TIMEOUT):
        self.host = host
        self.port = port
        self.server_db = server_db
        self.server_socket = socket(AF_INET, SOCK_STREAM)
        self.server_socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        if server_socket_timeout:
            self.server_socket.settimeout(server_socket_timeout)
        self.server_socket.bind((self.host, self.port))
        self.should_run = True
        self.client_handlers = []
        self.client_thread_lock = threading.Lock()

    def run(self, max_clients: int) -> None:
        logging.info(f'Starting server on {self.host}:{self.port}...')
        if not os.path.isdir(config.UPLOADED_FILES_DIRECTORY):
            logging.info(f"Uploaded files directory not exist. Creating it - '{config.UPLOADED_FILES_DIRECTORY}'")
            os.mkdir(config.UPLOADED_FILES_DIRECTORY)
        self.server_socket.listen(max_clients)
        while self.should_run:
            try:
                client_socket, address = self.server_socket.accept()
                logging.info(f'Accepted new connection: {address}')
                client_handler = ClientHandler(client_socket, self.server_db)
                Thread(target=client_handler.run).start()
                with self.client_thread_lock:
                    self.client_handlers.append(client_handler)
            except Exception as e:
                logging.error(f"Exception while accepting connection: {e}")

    def close(self) -> None:
        logging.info(f"Closing server")
        self.should_run = False
        if self.server_socket:
            logging.info(f"Closing server socket")
            self.server_socket.close()
        with self.client_thread_lock:
            logging.info(f"Closing all client handlers")
            for client_handler in self.client_handlers:
                if client_handler.is_running():
                    client_handler.close()
