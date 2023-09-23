from typing import List
from uuid import UUID
from dal.db_connection import DBConnection
from dal.models.client import Client


class ClientsTable(object):
    def __init__(self, db_connection: DBConnection) -> None:
        self._db_connection = db_connection

    def get_clients(self) -> List[Client]:
        return self._db_connection.select(Client)
    
    def _get_client(self, filters):
        rows = self._db_connection.select(Client, filters)
        if rows:
            return rows[0]
    
    def get_client_by_id(self, client_id: UUID) -> Client:
        filters = (Client.id == client_id,)
        return self._get_client(filters)

    def get_client_by_name(self, client_name: str) -> Client:
        filters = (Client.name == client_name,)
        return self._get_client(filters)

    def client_name_exists(self, client_name: str) -> bool:
        client = self.get_client_by_name(client_name)
        return client is not None

    def add_client(self, client: Client) -> None:
        self._db_connection.insert(client)

    def update_client_public_key(self, client_name: str, public_key: bytes) -> None:
        filters = (Client.name == client_name,)
        self._db_connection.update(Client, filters, "public_key", public_key)

    def update_client_aes_key(self, client_id: UUID, aes_key: bytes) -> None:
        filters = (Client.id == client_id,)
        self._db_connection.update(Client, filters, "aes_key", aes_key)
