#pragma once

#include <stdexcept>
#include "User.h"

class FriendAlreadyExistsException :
    public std::runtime_error
{
private:
    std::string username;
    std::string friendName;
public:
    FriendAlreadyExistsException(const std::string, const std::string);
    std::string getUserName();
    std::string getFriendName();
};
