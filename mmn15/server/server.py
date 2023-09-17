from logging import error, info
from socket import SOCK_STREAM, SOL_SOCKET, socket, SO_REUSEADDR
from threading import Thread


class Server(object):
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = socket(socket.AF_INET, SOCK_STREAM)
        self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        self.socket.bind((self.host, self.port))

    def run(self, max_clients):
        self.socket.listen(max_clients)
        while True:
            client_socket, address = self.socket.accept()
            info(f'Accepted new connection: {address}')
            client_socket.settimeout(60)
            Thread(target=self.handle_request, args=(client_socket,)).start()

    def handle_request(self, client_socket: socket):
        size = 1024
        receive_requests = True
        while receive_requests:
            try:
                data = self.client_socket.recv(size)
                if data:
                    # Process the data and send a response
                    response = data
                    self.client_socket.send(response)
                else:
                    raise error('Client disconnected')
            except Exception as exception:
                error(f'Closing client connection due to an error: {exception}')
                self.client_socket.close()
                receive_requests = False
