#pragma once
#include <map>
#include "User.h"
#include "Photo.h"
#include "Audio.h"
#include "Video.h"

using namespace std;

class USocial
{
private:
	unsigned long id_counter;
	map<unsigned long, User*> users;
public:
	User* registerUser(string, bool = false);
	void removeUser(User*);
	User* getUserById(unsigned long);
};
