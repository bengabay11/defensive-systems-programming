from logging import error, info, warning
import logging
import os

from server import Server
import config
from dal.db_connection import DBConnection
from dal.server_db import ServerDB


MAX_CLIENTS = 5
DEFAULT_PORT = 1357
PORT_FILE_PATH = os.path.join(os.path.dirname(__file__), 'port.info')
CHAT_SERVER_HOST = os.environ.get('HOST', '0.0.0.0')


def load_port(port_file_path: str):
    if os.path.exists(port_file_path):
        with open(port_file_path, 'r') as port_file:
            return int(port_file.read())
    else:
        warning(
            f"Port file not found: {port_file_path}. Using default port: {DEFAULT_PORT}")
        return DEFAULT_PORT


def main():
    logging.basicConfig(
        format=config.LOGGING_FORMAT,
        level=config.LOGGING_LEVEL,
        datefmt=config.LOGGING_DATE_FORMAT
    )
    db_connection = DBConnection()
    info(f"Connecting to database at {config.DB_PATH}")
    db_connection.connect(config.DB_PROTOCOL, config.DB_PATH)
    server_db = ServerDB(db_connection)
    info(f'Loading port from {PORT_FILE_PATH}')
    server_port = load_port(PORT_FILE_PATH)
    try:
        info(f'Starting server on {CHAT_SERVER_HOST}:{server_port}...')
        chat_server = Server(CHAT_SERVER_HOST, server_port, server_db)
        chat_server.run(MAX_CLIENTS)
    except Exception as exception:
        error(f"Aborting server due to an error: {exception}")
        exit(1)


if __name__ == '__main__':
    main()
