import os
from socket import socket
import struct
from uuid import UUID
import uuid

from request_handlers.base import BaseRequestHandler
from dal.server_db import ServerDB
from protocol import ResponseCodes
from protocol import RequestHeader
import cipher
import config
from dal.models.file import File
from crc import CRC32


METADATA_FORMAT = "<L255s"
FILE_CHUNK_SIZE = 1024

class FileUploadRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        metadata_buffer_size = struct.calcsize(METADATA_FORMAT)
        payload_metadata = client_socket.recv(metadata_buffer_size)
        content_size, filename_bytes = struct.unpack(METADATA_FORMAT, payload_metadata)
        filename = filename_bytes.decode().rstrip("\x00")
        encrypted_content = b""
        remaining_bytes = content_size
        while remaining_bytes > 0:
            block = client_socket.recv(FILE_CHUNK_SIZE)
            encrypted_content += block
            remaining_bytes -= len(block)
        client_id = UUID(bytes=request_header.client_id)
        client = server_db.clients.get_client_by_id(client_id)
        if client:
            decrypted_content = cipher.decrypt_bytes(client.aes_key, encrypted_content)
            FileUploadRequestHandler.save_file(decrypted_content, client_id, filename, server_db)
            content_crc = FileUploadRequestHandler.calculate_crc(decrypted_content)
            payload = struct.pack("<16sL255sL", client_id.bytes, len(decrypted_content), filename.encode(), content_crc)
            return ResponseCodes.VALID_FILE_ACCEPTED, payload
        else:
            pass
            # handle client not authorized

    @staticmethod
    def save_file(file_content: bytes, client_id: UUID, filename: str, server_db: ServerDB):
        base_folder = os.path.join(config.UPLOADED_FILES_DIRECTORY, str(uuid.uuid4()))
        os.mkdir(base_folder)
        file_path = os.path.join(base_folder, filename)
        file = File(id=client_id, file_name=filename, path_name=file_path, verified=False)
        server_db.files.insert_file(file)
        with open(file.path_name, "wb") as f:
            f.write(file_content)
            f.flush()

    @staticmethod
    def calculate_crc(content: bytes) -> int:
        crc = CRC32()
        crc.update(content)
        return crc.digest()
