import logging
import os
from socket import socket
import struct
from uuid import UUID

from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import Response, ResponseCodes
from protocol import RequestHeader
import cipher
import config
from dal.models.file import File
from crc import CRC32
from request_handlers.common import authenticate_client
from dal.models.client import Client


METADATA_FORMAT = "<L255s"
RESPONSE_FORMAT = "<16sL255sL"
FILE_CHUNK_SIZE = 1024


class FileUploadRequestHandler(BaseRequestHandler):
    def handle(self, client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> Response:
        metadata_buffer_size = struct.calcsize(METADATA_FORMAT)
        payload_metadata = client_socket.recv(metadata_buffer_size)
        content_size, filename_bytes = struct.unpack(METADATA_FORMAT, payload_metadata)
        filename = filename_bytes.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        client = authenticate_client(client_id, server_db)
        base_folder = os.path.join(config.UPLOADED_FILES_DIRECTORY, str(client_id))
        file = server_db.files.get_file(client_id, filename)
        override = file is not None
        content_crc, decrypted_content = self.upload_file(
            client_socket,
            client,
            base_folder,
            filename,
            content_size,
            server_db,
            override
        )
        payload = struct.pack(RESPONSE_FORMAT, client_id.bytes, len(decrypted_content), filename.encode(), content_crc)
        return Response(ResponseCodes.VALID_FILE_ACCEPTED, payload)

    def receive_file_content(self, client_socket: socket, content_size: int):
        encrypted_content = b""
        remaining_bytes = content_size
        while remaining_bytes > 0:
            block = client_socket.recv(FILE_CHUNK_SIZE)
            encrypted_content += block
            remaining_bytes -= len(block)
        return encrypted_content

    def upload_file(self, client_socket: socket, client: Client, base_folder: str, filename: str,content_size: int, server_db: ServerDB, override: bool):
        logging.info(f"Uploading file '{filename}' for client '{client.name}' to the server")
        encrypted_content = self.receive_file_content(client_socket, content_size)
        decrypted_content = cipher.decrypt_bytes(client.aes_key, encrypted_content)
        self.save_file(base_folder, decrypted_content, client.id, filename, server_db, override)
        content_crc = self.calculate_crc(decrypted_content)
        logging.info(f"CRC for file '{filename}' - {content_crc}")
        return content_crc, decrypted_content

    def save_file(self, base_folder: str, file_content: bytes, client_id: UUID, filename: str, server_db: ServerDB, override: bool):
        logging.debug(f"Saving file '{filename}' to folder '{base_folder}'")
        if not os.path.exists(base_folder):
            logging.info(f"Base folder does not exist for client {client_id}. Creating it now.")
            os.mkdir(base_folder)
        file_path = os.path.join(base_folder, filename)
        file = File(id=client_id, file_name=filename, path_name=file_path, verified=False)
        if override:
            logging.info(f"File is already exists in the DB, and therefor no DB changes needed.")
        if not override:
            logging.info(f"Saving New file to the DB - {file}")
            server_db.files.insert_file(file)
        with open(file_path, "wb") as file_object:
            file_object.write(file_content)
            file_object.flush()

    def calculate_crc(self, content: bytes) -> int:
        crc = CRC32()
        crc.update(content)
        return crc.digest()
