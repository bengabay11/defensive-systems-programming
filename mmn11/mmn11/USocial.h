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
	map<unsigned long, User*> users;
public:
	User* registerUser(string, bool);
	void removeUser(User*);
	User* getUserById(unsigned long);
};
