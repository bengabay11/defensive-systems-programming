#pragma once

#include <stdexcept>

class NotAFriendException :
    public std::runtime_error
{
private:
    std::string username;
    std::string otherUsername;
public:
    NotAFriendException(const std::string, const std::string);
    std::string getUserName();
    std::string getOtherUserName();
};
