#pragma once

#include "User.h"

class BusinessUser :
    public User
{
public:
    BusinessUser();
    void sendMessage(User* user, Message* message) override;
};
