#pragma once

#include <map>
#include "User.h"
#include "Photo.h"
#include "Audio.h"
#include "Video.h"

class USocial
{
private:
	unsigned long id_counter;
	std::map<unsigned long, User*> users;
public:
	User* registerUser(std::string, bool = false);
	void removeUser(User*);
	User* getUserById(unsigned long);
};
