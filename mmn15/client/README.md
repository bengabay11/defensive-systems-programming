# Client

This README provides detailed information about the client software in the client-server file transfer system. The client is written in C++ and is responsible for communicating with the server, registering users, exchanging encryption keys, and securely transferring files.

## Installation

- Install the required C++ development tools and libraries, including CryptoPP for encryption.

## Client Instructions

To use the client software, follow these instructions:

1. Create a file named `transfer.info` in the same folder as the client executable. This file should contain the following information:
   - First line: IP address + colon + port number (e.g., `127.0.0.1:1234`)
   - Second line: Customer's name (up to 100 characters)
   - Third line: Path of the file to be sent to the server

    Example:

    ```sh
    127.0.0.1:1234
    Michael Jackson
    New_product_spec.docx
    ``````

2. Create a file named `info.me` in the same folder as the client executable. This file should contain the following information:

    - First line: Name
    - Second line: Unique identifier in ASCII representation (hex value)
    - Third line: Private key created in the first run of the program in base format .64

    Example:

    ```sh
    Michael Jackson
    64f3f63985f04beb81a0e43321880182
    MIGdMA0GCSqGSIb3DQEBA…
    ``````

3. Run the client software. It will read the information from `transfer.info` and `me.info`, communicate with the server, and perform the file transfer.

## Error Handling

In case of an error, the client prints a message on the screen: "Error with the server response" and attempts to resend the message up to three times. If it still fails, the client exits with a detailed fatal message.

For further details on the client-server system, including the server-side components and general project information, please refer to the main [General Project README](../README.md).
