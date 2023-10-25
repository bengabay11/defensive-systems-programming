# File Transfer System

This README provides a high-level overview of the client-server file transfer system implemented as a university assignment. The system comprises a server written in Python and a client written in C++. The main goal is to enable customers to securely transfer files from their computer to the server, ensuring the encryption and integrity of the data.

## Software Architecture

The software architecture is based on a client-server model, where the client initiates communication with the server. Here are the key components and characteristics of the system:

- The client is responsible for contacting the server, exchanging encryption keys, and transferring files over an encrypted channel.
- The server is written in Python and supports multiple users through threads.
- The server version is 3, which is included in communication messages.
- Encryption is performed using the `Cipher.Crypto` encryption package.
- Port number for server communication is read from a text file named `port.info` in the same folder as the server's code files. If the file is not available, a default port of 1357 is used.

## Server and Client Details

For more detailed information about the server and client components, please refer to the specific README files:

- [Server README](server/README.md): Detailed information about the server, including data storage, operation, and database structure.
- [Client README](client/README.md): Detailed information about the client software, including client instructions, data storage, and error handling.

## Additional Information

- The system uses a universally unique identifier (UUID) for customer identification.
- For further reading on UUIDs, visit [this link](https://en.wikipedia.org/wiki/Universally_unique_identifier).
