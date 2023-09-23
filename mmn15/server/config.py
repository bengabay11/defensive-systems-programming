import logging
import os


VERSION = 3
UPLOADED_FILES_DIRECTORY = os.path.join(os.getcwd(), "server", "files")

# Logging
LOGGING_LEVEL = logging.DEBUG
LOGGING_FORMAT = '[%(asctime)s] %(levelname)s - %(message)s'
LOGGING_DATE_FORMAT = '%Y-%m-%d %H:%M:%S'

# Database Connection
DB_PROTOCOL = "sqlite"
current_file_path = os.path.abspath(__file__)
server_folder = os.path.dirname(current_file_path)
DB_PATH = os.path.join(server_folder, "defensive.db")
