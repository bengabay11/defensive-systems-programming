#pragma once

#include <map>
#include "User.h"
#include "Photo.h"
#include "Audio.h"
#include "Video.h"

class USocial
{
private:
	unsigned long idCounter;
	std::map<unsigned long, User*> users;
public:
	USocial();
	~USocial();
	USocial(const USocial& other);
	USocial& operator=(const USocial& other);
	User* registerUser(std::string username, bool isBusiness = false);
	void removeUser(User* user);
	User* getUserById(unsigned long userId);
};
