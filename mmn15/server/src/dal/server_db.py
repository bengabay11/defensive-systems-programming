from pattern_singleton import Singleton
from dal.db_connection import DBConnection
from dal.tables.clients import ClientsTable
from dal.tables.files import FilesTable


class ServerDB(metaclass=Singleton):
    def __init__(self, db_connection: DBConnection) -> None:
        self._db_connection = db_connection
        self._clients = ClientsTable(self._db_connection)
        self._files = FilesTable(self._db_connection)

    @property
    def clients(self) -> ClientsTable:
        return self._clients

    @property
    def files(self) -> FilesTable:
        return self._files

    def close(self) -> None:
        self._db_connection.close()
