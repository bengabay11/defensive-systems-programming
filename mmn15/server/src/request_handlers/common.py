import datetime
import logging
from uuid import UUID
from dal.server_db import ServerDB
from errors import ClientNotFoundError


def authenticate_client(client_id: UUID, server_db: ServerDB):
    logging.info(f"Authenticating client with id '{client_id}'")
    client = server_db.clients.get_client_by_id(client_id)
    if client:
        server_db.clients.update_last_seen(client_id, datetime.datetime.now())
        return client
    else:
        raise ClientNotFoundError(client_id)
