from logging import debug, info
import os
from socket import socket
import struct
from uuid import UUID

from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader
import cipher
import config
from dal.models.file import File
from crc import CRC32
from errors import FileAlreadyExistsError
from request_handlers.common import authenticate_client, delete_file
from dal.models.client import Client


METADATA_FORMAT = "<L255s"
FILE_CHUNK_SIZE = 1024


class FileUploadRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        metadata_buffer_size = struct.calcsize(METADATA_FORMAT)
        payload_metadata = client_socket.recv(metadata_buffer_size)
        content_size, filename_bytes = struct.unpack(METADATA_FORMAT, payload_metadata)
        filename = filename_bytes.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        client = authenticate_client(client_id, server_db)
        base_folder = os.path.join(config.UPLOADED_FILES_DIRECTORY, str(client_id))
        file = server_db.files.get_file(client_id, filename)
        if not file:
            content_crc, decrypted_content = self.upload_file(
                client_socket,
                client,
                base_folder,
                filename,
                content_size,
                server_db
            )
            payload = struct.pack("<16sL255sL", client_id.bytes, len(decrypted_content), filename.encode(), content_crc)
            return ResponseCodes.VALID_FILE_ACCEPTED, payload
        else:
            raise FileAlreadyExistsError(client_id, filename)

    def receive_file_content(self, client_socket: socket, content_size: int):
        encrypted_content = b""
        remaining_bytes = content_size
        while remaining_bytes > 0:
            block = client_socket.recv(FILE_CHUNK_SIZE)
            encrypted_content += block
            remaining_bytes -= len(block)
        return encrypted_content

    def upload_file(self, client_socket: socket, client: Client, base_folder: str, filename: str,content_size: int, server_db: ServerDB):
        info(f"Uploading file '{filename} for client '{client.name} to the server'")
        encrypted_content = self.receive_file_content(client_socket, content_size)
        decrypted_content = cipher.decrypt_bytes(client.aes_key, encrypted_content)
        self.save_file(base_folder, decrypted_content, client.id, filename, server_db)
        content_crc = self.calculate_crc(decrypted_content)
        info(f"CRC for file '{filename}' - {content_crc}")
        return content_crc, decrypted_content

    def save_file(self, base_folder: str, file_content: bytes, client_id: UUID, filename: str, server_db: ServerDB):
        debug(f"Saving file '{filename}' to folder '{base_folder}'")
        if not os.path.exists(base_folder):
            info(f"Base folder does not exist for client {client_id}. Creating it now.")
            os.mkdir(base_folder)
        file_path = os.path.join(base_folder, filename)
        file = File(id=client_id, file_name=filename, path_name=file_path, verified=False)
        server_db.files.insert_file(file)
        with open(file_path, "wb") as file_object:
            file_object.write(file_content)
            file_object.flush()

    def calculate_crc(self, content: bytes) -> int:
        crc = CRC32()
        crc.update(content)
        return crc.digest()
