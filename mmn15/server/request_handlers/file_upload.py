from logging import debug, error
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
from errors import ClientNotFoundError
from errors import FileAlreadyExistsError


METADATA_FORMAT = "<L255s"
FILE_CHUNK_SIZE = 1024

class FileUploadRequestHandler(BaseRequestHandler):
    @staticmethod
    def handle(client_socket: socket, request_header: RequestHeader, server_db: ServerDB) -> (ResponseCodes, bytes):
        metadata_buffer_size = struct.calcsize(METADATA_FORMAT)
        payload_metadata = client_socket.recv(metadata_buffer_size)
        content_size, filename_bytes = struct.unpack(METADATA_FORMAT, payload_metadata)
        filename = filename_bytes.decode().rstrip("\x00")
        client_id = UUID(bytes=request_header.client_id)
        base_folder = os.path.join(config.UPLOADED_FILES_DIRECTORY, str(uuid.uuid4()))
        try:
            content_crc, decrypted_content = FileUploadRequestHandler.upload_file(
                client_socket,
                base_folder,
                client_id,
                filename,
                content_size,
                server_db
            )
            payload = struct.pack("<16sL255sL", client_id.bytes, len(decrypted_content), filename.encode(), content_crc)
            return ResponseCodes.VALID_FILE_ACCEPTED, payload
        except Exception as e:
            FileUploadRequestHandler.rollback_upload_file(base_folder, client_id, filename, server_db)
            raise e

    @staticmethod
    def receive_file_content(client_socket: socket, content_size: int):
        encrypted_content = b""
        remaining_bytes = content_size
        while remaining_bytes > 0:
            block = client_socket.recv(FILE_CHUNK_SIZE)
            encrypted_content += block
            remaining_bytes -= len(block)
        return encrypted_content

    @staticmethod
    def upload_file(client_socket: socket, base_folder: str, client_id: UUID, filename: str, content_size: int, server_db: ServerDB):
        encrypted_content = FileUploadRequestHandler.receive_file_content(client_socket, content_size)
        file = server_db.files.get_file(client_id, filename)
        if not file:
            client = server_db.clients.get_client_by_id(client_id)
            if client:
                decrypted_content = cipher.decrypt_bytes(client.aes_key, encrypted_content)
                FileUploadRequestHandler.save_file(base_folder, decrypted_content, client_id, filename, server_db)
                content_crc = FileUploadRequestHandler.calculate_crc(decrypted_content)
                return content_crc, decrypted_content
            else:
                raise ClientNotFoundError(client_id)
        else:
            raise FileAlreadyExistsError(client_id, filename)

    @staticmethod
    def rollback_upload_file(base_folder, client_id: UUID, filename: str, server_db: ServerDB):
        if os.path.isdir(base_folder):
            error(f"Upload file failed. removing folder - '{base_folder}'")
            os.rmdir(base_folder)


    @staticmethod
    def save_file(base_folder: str, file_content: bytes, client_id: UUID, filename: str, server_db: ServerDB):
        debug(f"Saving file '{filename}' to folder '{base_folder}'")
        os.mkdir(base_folder)
        file_path = os.path.join(base_folder, filename)
        file = File(id=client_id, file_name=filename, path_name=file_path, verified=False)
        server_db.files.insert_file(file)
        with open(file_path, "wb") as file_object:
            file_object.write(file_content)
            file_object.flush()

    @staticmethod
    def calculate_crc(content: bytes) -> int:
        crc = CRC32()
        crc.update(content)
        return crc.digest()
