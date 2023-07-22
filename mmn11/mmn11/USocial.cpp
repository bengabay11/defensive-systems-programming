#include "USocial.h"
#include "BusinessUser.h"

/**
 * @brief Register a new user for USocial.
 * 
 * The function create user objects and adds it to the list of users.
 * @param username The name of the new user.
 * @param is_business Boolean param that checks if the user should be a BusinessUser or User.
 * @return The new user.
 */
User* USocial::registerUser(string username, bool is_business)
{
	User* new_user;
	if (is_business) {
		new_user = new BusinessUser();
	}
	else {
		new_user = new User();
	}
	new_user->id = id_counter++;
	new_user->name = username;
	new_user->us = this;
	this->users.insert(make_pair(new_user->getId(), new_user));
	return new_user;
}

/**
 * @brief Remove user from the list of users.
 * @param user The user to remove.
 */
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

/**
 * @brief Get user by it's id from the list of users.
 *
 * The function checks if there is a user with ID that matches the given ID in the list of users.
 * if there is, the function returns the user, if not the function return nullptr.
 * @param user_id The id of the requested user.
 * @return The user that matches the given id from the list of users.
 */
User* USocial::getUserById(unsigned long user_id)
{
	auto it = this->users.find(user_id);
	if (it != this->users.end()) {
		return it->second;
	}
	return nullptr;
}
