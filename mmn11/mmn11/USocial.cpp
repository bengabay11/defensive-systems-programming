#include "USocial.h"

User* USocial::registerUser(string username, bool)
{
	User* new_user = new User();
	unsigned long user_id = 1;
	new_user->id = user_id;
	new_user->us = this;
	this->users.insert(make_pair(user_id, new_user));
	return new_user;
}

void USocial::removeUser(User* user)
{
	for (auto it = this->users.begin(); it != this->users.end();) {
		if (it->second == user) {
			it = this->users.erase(it);
		}
		else {
			++it;
		}
	}
}

User* USocial::getUserById(unsigned long user_id)
{
	auto it = this->users.find(user_id);
	if (it != this->users.end()) {
		return it->second;
	}
}
