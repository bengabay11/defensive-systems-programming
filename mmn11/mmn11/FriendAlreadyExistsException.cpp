#include "FriendAlreadyExistsException.h"

FriendAlreadyExistsException::FriendAlreadyExistsException(std::string username, std::string friendName): 
	std::runtime_error(username + " is already a friend of " + friendName), username(username), friendName(friend_name) {}

std::string FriendAlreadyExistsException::getUserName()
{
	return this->username;
}

std::string FriendAlreadyExistsException::getFriendName()
{
	return this->friendName;
}
