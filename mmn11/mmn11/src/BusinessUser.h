#pragma once
#include "User.h"

class BusinessUser :
    public User
{
public:
    void SendMessage(User* user, Message* message);
};
