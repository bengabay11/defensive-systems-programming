#include "USocial.h"
#include "BusinessUser.h"

USocial::USocial() : idCounter(0) {}

USocial::~USocial()
{
	for (auto it = users.begin(); it != users.end(); ++it) {
		delete it->second;
	}
}

USocial::USocial(const USocial& other): idCounter(other.idCounter)
{
	for (const auto& pair : other.users) {
		users[pair.first] = new User(*pair.second);
	}
}

USocial& USocial::operator=(const USocial& other)
{
	if (this == &other) {
		return *this;
	}

	for (auto it = users.begin(); it != users.end(); ++it) {
		delete it->second;
	}

	this->users.clear();
	for (const auto& pair : other.users) {
		this->users[pair.first] = new User(*pair.second);
	}

	this->idCounter = other.idCounter;

	return *this;
}

/**
 * @brief Register a new user for USocial.
 * 
 * The function creates user object and adds it to the list of users.
 * @param username The name of the new user.
 * @param isBusiness Boolean param that checks if the user should be a BusinessUser or not.
 * @return The new user.
 */
User* USocial::registerUser(std::string username, bool isBusiness)
{
	User* newUser;
	if (isBusiness) {
		newUser = new BusinessUser(this, this->idCounter++, username);
	}
	else {
		newUser = new User(this, this->idCounter++, username);
	}
	this->users.insert(std::make_pair(newUser->getId(), newUser));
	return newUser;
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
 * @param userId The id of the requested user.
 * @return The user that matches the given id from the list of users.
 */
User* USocial::getUserById(unsigned long userId)
{
	auto it = this->users.find(userId);
	if (it != this->users.end()) {
		return it->second;
	}
	return nullptr;
}
