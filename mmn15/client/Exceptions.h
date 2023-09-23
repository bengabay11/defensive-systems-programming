#pragma once

#include <stdexcept>
#include <stdint.h>
#include <cstdio>

#include "Consts.h"

class GeneralServerErrorException:
    public std::runtime_error
{
public:
    GeneralServerErrorException();
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

class OpenFileFailException:
    public std::runtime_error
{
private:
    std::string filePath;
public:
    OpenFileFailException(std::string filePath);
};
