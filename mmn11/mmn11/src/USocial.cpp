#include "USocial.h"

User* USocial::registerUser(string username, bool)
{
	User* new_user = new User();
	unsigned long user_id = 1;
	this->users.insert(make_pair(user_id, new_user));
}
