#include "NotAFriendException.h"

NotAFriendException::NotAFriendException(const std::string username, const std::string otherUsername): 
	runtime_error(username + " is not a friend of " + otherUsername), username(username), otherUsername(otherUsername) {}

std::string NotAFriendException::getUserName()
{
	return this->username;
}

std::string NotAFriendException::getOtherUserName()
{
	return this->otherUsername;
}

