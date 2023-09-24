#pragma once

#include <stdexcept>
#include <stdint.h>
#include <cstdio>

#include "Consts.h"

class SendRequestMaxRetriesException:
    public std::runtime_error
{
public:
    SendRequestMaxRetriesException();
};

class UnkownResponseCodeException:
    public std::runtime_error
{
private:
    short responseCode;
public:
    UnkownResponseCodeException(short responseCode);
};

class LoginDeclinedException:
    public std::runtime_error
{
public:
    LoginDeclinedException();
};

class RegistrationFailedException:
    public std::runtime_error
{
private:
    std::string clientName;
public:
    RegistrationFailedException(std::string clientName);
};

class OpenFileException:
    public std::runtime_error
{
private:
    std::string filePath;
public:
    OpenFileException(std::string filePath);
};

class ConnectServerException :
    public std::runtime_error
{
private:
    std::string host;
    int port;
    static std::string generateErrorMessage(const std::string& host, int port);
public:
    ConnectServerException(std::string host, int port);
};

class SocketSendException:
    public std::runtime_error
{
public:
    SocketSendException();
};

class SocketRecvException :
    public std::runtime_error
{
public:
    SocketRecvException();
};

class InitWinsockException:
    public std::runtime_error
{
public:
    InitWinsockException();
};

class CreateSocketException:
    public std::runtime_error
{
private:
    int WSALastError;
public:
    CreateSocketException(int WSALastError);
};