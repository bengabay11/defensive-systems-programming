# Server

This README provides detailed information about the server software in the client-server file transfer system. The server is written in Python and is responsible for managing user registrations, exchanging encryption keys, and securely receiving and storing files from clients.

## Installation

- Install Python 3.11.4 and the required packages, including `Cipher.Crypto` for encryption.

## Data Storage

The server stores customer data and files in memory (RAM) and manages an SQLite database named `db.defensive`. This database includes two tables: `clients` and `files`.

### `clients` Table Structure

- `ID`: 16 bytes (128 bits) unique identifier for each customer.
- `Name`: String (255 characters) representing the user's name.
- `PublicKey`: 160 bytes public key of the client.
- `LastSeen`: Date and time of the last request from the client.
- `AES Key`: 128-bit AES key sent to the client.

### `files` Table Structure

- `ID`: 16 bytes (128 bits) unique identifier for each file.
- `Name`: String (255 bytes) representing the file name as sent from the user.
- `Path`: String (255 bytes) representing the relative path and file name stored on the server.
- `Verified`: Boolean indicating whether the checksum was successfully verified against the customer.

## Server Operation

1. The server reads the port from the `port.info` file. If the file does not exist, a warning is issued, and the default port (1357) is used.
2. It checks if the `db.defensive` database already exists and loads customer data recorded in previous sessions.
3. The server waits for requests from customers in an endless loop.
4. When receiving a request, it processes it according to the protocol:
   - Registration request: If the requested username already exists, the server returns an error. Otherwise, a new UUID is generated, and the data is saved in memory and the database. A success response is sent.
   - Public key from a client is captured and updated in the database. In response, the server generates an AES key, encrypts it with the public key, and sends it back to the client.
   - Message with an encrypted file: The server decrypts the file using the original AES key sent to the customer and calculates the CRC (checksum). The server expects a success message from the client (CRC verified) or retries sending the file up to three times if verification fails.

For more information on the general project and the client-side components, please refer to the main [General Project README](../README.md).
