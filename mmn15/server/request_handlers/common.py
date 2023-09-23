

from logging import debug
import os
from uuid import UUID
from dal.server_db import ServerDB
from errors import ClientNotFoundError


def delete_file(client_id: UUID, filename: str, server_db: ServerDB) -> None:
    file = server_db.files.get_file(client_id, filename)
    if file:
        base_folder = os.path.dirname(file.path_name)
        debug(f"Invalid CRC for file - '{filename}'. Removing base folder - '{base_folder}' and deleting the file from DB")
        server_db.files.delete_file(client_id, filename)
        if os.path.isfile(file.path_name):
            os.remove(file.path_name)
        if os.path.isdir(base_folder):
            os.rmdir(base_folder)


def authenticate_client(client_id: UUID, server_db: ServerDB):
    client = server_db.clients.get_client_by_id(client_id)
    if client:
        return client
    else:
        raise ClientNotFoundError(client_id)
