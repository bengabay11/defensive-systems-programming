import logging
import os


VERSION = 3
UPLOADED_FILES_DIRECTORY = os.path.join(os.getcwd(), "files")

# Logging
LOGGING_LEVEL = logging.DEBUG
LOGGING_FORMAT = '[%(asctime)s] %(levelname)s - %(message)s'
LOGGING_DATE_FORMAT = '%Y-%m-%d %H:%M:%S'

# Database Connection
DB_PROTOCOL = "sqlite"
current_file_path = os.path.abspath(__file__)
server_folder = os.path.dirname(current_file_path)
DB_PATH = os.path.join(server_folder, "defensive.db")

# Server Connection
MAX_CLIENTS = 5
DEFAULT_PORT = 1357
PORT_FILE_PATH = os.path.join(os.path.dirname(__file__), 'port.info')
SERVER_HOST = os.environ.get('HOST', '0.0.0.0')
DEFAULT_SERVER_SOCKET_TIMEOUT = None
DEFAULT_CLIENT_SOCKET_TIMEOUT = 60

