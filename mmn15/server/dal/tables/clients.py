

from typing import List
from server.dal.db_connection import DBConnection
from server.dal.models.client import Client


class ClientsTable(object):
    def __init__(self, db_connection: DBConnection) -> None:
        self._db_connection = db_connection

    def get_clients(self, serializable: bool = False) -> List[Client]:
        return self._db_connection.select(Client, serializable=serializable)

    def add_client(self, client: Client) -> None:
        pass

    def update_client_public_key(self, client_id: int, public_key: bytes) -> None:
        self._db_connection.update(Client, client_id=client_id, public_key=public_key)
