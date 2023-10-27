import logging
import os

from server import Server
import config
from dal.db_connection import DBConnection
from dal.server_db import ServerDB


def load_port(port_file_path: str):
    if os.path.exists(port_file_path):
        with open(port_file_path, 'r') as port_file:
            return int(port_file.read())
    else:
        logging.warning(
            f"Port file not found: {port_file_path}. Using default port: {config.DEFAULT_PORT}")
        return config.DEFAULT_PORT


def main():
    logging.basicConfig(
        format=config.LOGGING_FORMAT,
        level=config.LOGGING_LEVEL,
        datefmt=config.LOGGING_DATE_FORMAT
    )
    db_connection = DBConnection()
    logging.info(f"Connecting to database at {config.DB_PATH}")
    db_connection.connect(config.DB_PROTOCOL, config.DB_PATH)
    server_db = ServerDB(db_connection)
    logging.info(f"Loading port from {config.PORT_FILE_PATH}")
    server_port = load_port(config.PORT_FILE_PATH)
    try:
        server = Server(config.SERVER_HOST, server_port, server_db)
        server.run(config.MAX_CLIENTS)
    except Exception as exception:
        logging.error(f"Aborting server due to an exception: {exception}")
        server.close()
        exit(1)


if __name__ == '__main__':
    main()
