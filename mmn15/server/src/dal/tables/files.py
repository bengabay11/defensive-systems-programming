from uuid import UUID
from dal.db_connection import DBConnection
from dal.models.file import File


class FilesTable(object):
    def __init__(self, db_connection: DBConnection) -> None:
        self._db_connection = db_connection

    def get_file(self, client_id: UUID, filename: str) -> File:
        filters = (File.id == client_id, File.file_name == filename,)
        rows = self._db_connection.select(File, filters)
        if rows:
            return rows[0]

    def delete_file(self, client_id: UUID, filename: str) -> None:
        filters = (File.id == client_id, File.file_name == filename,)
        self._db_connection.delete(File, filters)

    def update_file_verified(self, client_id: UUID, filename: str, verified: bool) -> None:
        filters = [File.id == client_id, File.file_name == filename]
        self._db_connection.update(File, filters, "verified", verified)

    def insert_file(self, file: File) -> None:
        self._db_connection.insert(file)
