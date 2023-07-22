#pragma once
#include "User.h"

class BusinessUser :
    public User
{
public:
    BusinessUser(unsigned long, USocial*, string);
    void sendMessage(User*, Message*) override;
};
