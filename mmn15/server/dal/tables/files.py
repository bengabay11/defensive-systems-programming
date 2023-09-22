from typing import List
from uuid import UUID
from dal.db_connection import DBConnection
from dal.models.client import Client
from dal.models.file import File


class FilesTable(object):
    def __init__(self, db_connection: DBConnection) -> None:
        self._db_connection = db_connection

    def delete_file(self, client_id: UUID, filename: str) -> None:
        pass

    def update_file(self):
        pass

    def insert_file(self, file: File) -> None:
        self._db_connection.insert(file)