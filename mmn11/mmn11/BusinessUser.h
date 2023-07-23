#pragma once

#include "User.h"

class BusinessUser :
    public User
{
public:
    BusinessUser(USocial* us, unsigned long id, std::string name);
    void sendMessage(User* user, Message* message) override;
};
