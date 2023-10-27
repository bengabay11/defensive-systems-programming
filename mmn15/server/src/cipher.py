from Crypto import Random
from Crypto.Cipher import PKCS1_OAEP, AES
from Crypto.PublicKey import RSA
from Crypto.Util.Padding import unpad


AES_KEY_LENGTH = int(128 / 8)  # 16 bytes
DEFAULT_IV = b'\0' * 16


def generate_aes_key() -> bytes:
    return Random.get_random_bytes(AES_KEY_LENGTH)


def encrypt_aes_key(aes_key: bytes, client_public_key: bytes) -> bytes:
    imported_key = RSA.importKey(client_public_key)
    cipher = PKCS1_OAEP.new(imported_key)
    return cipher.encrypt(aes_key)


def decrypt_bytes(aes_key: bytes, content: bytes) -> bytes:
    cipher = AES.new(aes_key, AES.MODE_CBC, DEFAULT_IV)
    return unpad(cipher.decrypt(content), AES.block_size)
