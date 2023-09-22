from typing import List
from dal.db_connection import DBConnection
from dal.models.client import Client


class ClientsTable(object):
    def __init__(self, db_connection: DBConnection) -> None:
        self._db_connection = db_connection

    def get_clients(self) -> List[Client]:
        return self._db_connection.select(Client)

    def client_name_exists(self, client_name: str) -> bool:
        filters = (Client.name == client_name,)
        rows = self._db_connection.select(Client, filters)
        return len(rows) > 0

    def add_client(self, client: Client) -> None:
        self._db_connection.insert(client)

    def update_client_public_key(self, client_id: int, public_key: bytes) -> None:
        self._db_connection.update(Client, client_id=client_id, public_key=public_key)
