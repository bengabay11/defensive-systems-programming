#include "USocial.h"
#include "BusinessUser.h"

User* USocial::registerUser(string username, bool is_business)
{
	User* new_user;
	if (is_business) {
		new_user = new BusinessUser(id_counter++, this, username);
	}
	else {
		new_user = new User(id_counter++, this, username);
	}
	this->users.insert(make_pair(new_user->getId(), new_user));
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
