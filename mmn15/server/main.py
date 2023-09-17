from logging import error, info
import os

from server.server import Server

MAX_CLIENTS = 5
DEFAULT_PORT = 1357
PORT_FILE_PATH = os.path.join(os.path.dirname(__file__), 'port.info')
CHAT_SERVER_HOST = os.environ.get('HOST', '0.0.0.0')


def load_port(port_file_path: str):
    if os.path.exists(port_file_path):
        with open(port_file_path, 'r') as port_file:
            return int(port_file.read())
    return DEFAULT_PORT



def main():
    info(f'Loading port from {PORT_FILE_PATH}')
    server_port = load_port()
    try:
        info(f'Starting server on {CHAT_SERVER_HOST}:{server_port}...')
        chat_server = Server(CHAT_SERVER_HOST, server_port)
        chat_server.run(MAX_CLIENTS)
    except Exception as exception:
        error(f"Aborting server due to an error: {exception}")
        exit(1)


if __name__ == '__main__':
    main()
